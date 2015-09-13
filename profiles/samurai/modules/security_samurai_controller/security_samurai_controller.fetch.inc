<?php
/**
 * @file
 * functions to retrieve available update data for the modules and themes
 *
 * @author jkswoods
 */

/**
 * Performs a curl request
 *
 * @param string $url The url of the site to check.
 *
 * @return string $content The content of the page.
 */
function samurai_curl_request($url) {

  // Create a new curl object
  $curl = curl_init();

  if (variable_get('samurai_debug', FALSE)) {
    curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, 0);
  }

  curl_setopt($curl, CURLOPT_URL, $url);
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

  // Execute the request.
  $content = curl_exec($curl);

  // Return the result.
  return $content;
}

/**
 * Performs a curl request and creates a SimpleXML object from the content.
 *
 * @param string $url The url of the site to check.
 *
 * @return SimpleXMLElement[object] $xml The content of the page.
 */
function samurai_xml_curl_request($url) {

  $content = samurai_curl_request($url);
  // Create the XML object.
  try {
    $xml = new SimpleXMLElement($content);
  } catch (Exception $e) {
    // SimpleXML handles errors itself.
    return NULL;
  }
  // Return the result.
  return $xml;
}

/**
 * Implements hook_cron
 */
function security_samurai_controller_cron() {

  // Check the SA based on time
  $last_sa_check = variable_get('security_samurai_controller_last_sa_check', 0);
  $next_sa_check = variable_get('security_samurai_controller_next_sa_check', 0);
  if (REQUEST_TIME >= $next_sa_check) {

    // Retrieve the SA check interval variable
    $interval_sa_check = strtotime(variable_get('security_samurai_controller_sa_check_interval', '5 minutes'));

    // Update the interval variables
    variable_set('security_samurai_controller_last_sa_check', REQUEST_TIME);
    variable_set('security_samurai_controller_next_sa_check', $interval_sa_check);

    // Retrieve the contrib sa XML
    $xml_contrib = samurai_xml_curl_request(SAMURAI_ORG_SA_CONTRIB);
    if (!is_null($xml_contrib)) {
      // Process the contrib projects security announcements.
      samurai_process_sa($xml_contrib, 'contrib');
    }
    // Retrieve the core sa XML
    $xml_core = samurai_xml_curl_request(SAMURAI_ORG_SA_CORE);
    if (!is_null($xml_core)) {
      // Process the core projects security announements.
      samurai_process_sa($xml_core, 'core');
    }

  }

  // Get an array of sites that have not yet had their sites checked/need to be checked.
  $result = db_query("SELECT id FROM {securitysamurai_site}
    WHERE update_next_check <= :time",
    array(
      ':time' => REQUEST_TIME,
  ));

  // Loop through each result.
  foreach ($result as $record) {

    // Load the site.
    $site = client_site_load($record->id);

    // The retrieval of update data for each site is slightly different
    // depending on the method
    // $site->update_check_method can have two values:
    // - 1 = encryption key
    // - 2 = drush aliases
    // In future this should become extensible, allowing for custom
    // update retrieval methods
    if ($site->update_check_method == 1) {

      // -- Get the update data from the site.
      // -- Decode the json encoded update data, to use as a PHP array.
      // -- Decrypt the update data.
      $encoded_update_data = samurai_get_site_update_data($site->site_url);
      $encrypted_update_data = drupal_json_decode($encoded_update_data);
      $decrypted_update_data = drupal_json_decode(
      trim(samurai_decrypt_update_data(
        $encrypted_update_data['encrypted'],
        $site->encryption_key)));

      // Assign values to the loaded entity
      $site->site_update_data = drupal_json_encode(samurai_process_update_data(array_reverse($decrypted_update_data)));
      $site->update_count = samurai_count_available_security_updates($decrypted_update_data);
      $site->update_next_check = strtotime(variable_get('security_samurai_controller_site_check_interval', '12 hours'));
      $site->core_version = samurai_get_site_core_major($site->name, $site->site_update_data);
    } elseif ($site->update_check_method == 2) {

      // Initialise a new SamuraiDocker object
      $aliases = new SamuraiDocker();

      // -- Get the update data from the site via drush (drush @alias pm-list)
      // -- Format the retrieved data into an array
      //
      $unformatted_update_data = $aliases->invoke_process($site->site_alias_name, 'pm-list');
      $formatted_update_data = samurai_process_drush_update_data($unformatted_update_data['output']);

      // Assign values to the loaded entity
      $site->site_update_data = drupal_json_encode($formatted_update_data);
      $site->update_count = samurai_count_available_security_updates($formatted_update_data);
      $site->update_next_Check = strtotime(variable_get('security_samurai_controller_site_check_interval', '12 hours'));
      $site->core_version = samurai_get_site_core_major($site->name, $site->site_update_data);
    }

    // Save the entity
    entity_save('client_site', $site);

    // Log to watchdog.
    watchdog('Security samurai', t('@name has been scanned.', array('@name' => $site->name)));
  }

  // Get an array of projects that require checking for a general update.
  $projects = db_query("SELECT id FROM {securitysamurai_projects}
    WHERE update_next_check <= :time",
    array(
      ':time' => REQUEST_TIME,
  ))->fetchAll();

  // Loop through each result.
  foreach ($projects as $record) {

    // Load the project.
    $project = project_load($record->id);

    // -- Get the machine name.
    // -- Get the major version.
    // -- Get the compatible core version.
    $machine_name = $project->machine_name;
    $major_version = $project->major_version;
    $core = $project->core;

    // Save the entity with a new time to check for another update.
    $project->update_next_check = strtotime(variable_get('security_samurai_controller_module_check_interval', '1 hour'));
    entity_save('project', $project);

    // Get the latest project versions.
    samurai_get_project_update($machine_name, $major_version, $core);
  }
}

/**
 * Process the security announcment feed
 *
 * @param SimpleXMLElement[object] The sa feed.
 * @param string $type core or contrib.
 */
function samurai_process_sa($xml, $type) {

  // Loop through each item in the security announcement XML
  foreach ($xml->channel->item as $item) {
    // RSS sometimes isn't recognised properly in SimpleXML
    foreach ($item as $key => $value) {
      $item->$key = strip_tags($value);
    }
    // The description contains everything needed and will need to be split up.
    $description = (string)$item->description;

    // Parse the security announcement ID.
    $sa = samurai_split_sa_data($description, 'Advisory ID:', 'Project:');

    // Check the security announcement doesn't already exist.
    if (!samurai_sa_exists($sa)) {
      // Security announcement doesn't exist.
      //
      // Get:
      // -- The core version affected
      // -- The risk level as a string
      // -- Time of announcement time
      // -- Vulnerability description
      // -- The solution to the vulnerabilty
      if ($type == 'contrib') {
        $project_name = samurai_split_sa_data($description, 'Project:', '(third-party module)');
      } else {
        $project_name = 'Drupal core';
      }
      $version_aff_raw = samurai_split_sa_data($description, 'Version:', 'Date:');
      $version_aff_json = drupal_json_encode(explode(',', $version_aff_raw));
      $version_aff = str_replace(' ', '', $version_aff_json);
      $risk_level = samurai_split_sa_data($description, 'Security risk:', 'Vulnerability:');
      $time = strtotime(samurai_split_sa_data($description, 'Date:', 'Security risk:'));
      $vulnerability = samurai_split_sa_data($description, 'Vulnerability:', 'CVE identifier(s)');
      $solution  = samurai_split_sa_data($description, 'Solution', 'Reported by');

      // Create and save the new SA entity.
      $security_announcement = entity_create('security_announcement', array(
        'sa_id' => $sa,
        'risk_level' => $risk_level,
        'date' => $time,
        'version' => $version_aff,
        'vulnerability' => $vulnerability,
        'solution' => $solution,
        'project_name' => $project_name,
        'secure_minor' => 'EMPTY'
      ));
      $sa_wrapped = entity_metadata_wrapper('security_announcement', $security_announcement);
      $sa_wrapped->save();

      // Log the creation of the SA entity to watchdog.
      watchdog('Security samurai', $sa . t(' New security update data'));

      // Check for a security update concerning the project_name from the SA.
      $core = drupal_json_decode($version_aff);
      if (!empty($machine_name)) {

        // Check update for each core version
        foreach ($core as $c => $core_major) {

          $am = samurai_get_available_major_versions($machine_name, $core_major);

          if (is_array($am)) {
            foreach ($am as $available_major) {

              // Retrieve the update.
              samurai_get_project_update($machine_name, $available_major, $core_major);
            }
          } else {
            samurai_get_project_update($machine_name, $am, $core_major);
          }
        }
      }
    } else {
      continue;
    }
  }
}

/**
 * Samurai get project update
 *
 * The way this function works is:
 *  - Checks for an update on at least each project once every day
 *  - It will then load and update the entity with the updated data.
 *  - Only check updates for projects that are supported.
 *
 * @param string $machine_name The machine name of the project.
 * @param int $major_version The major_version of the project.
 * @param string $core The core version that the project is for e.g. 7.x
 */
function samurai_get_project_update($machine_name, $major_version, $core, $xml = NULL) {

  if (is_null($xml)) {
    // Retrieve the update data.
    $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . "/" . $core);
  }

  // Get the latest version and secure version.
  $latest_secure_version = samurai_get_project_latest_secure_version($machine_name, $core, $major_version, $xml);
  $latest_version = samurai_get_project_latest_version($machine_name, $core, $major_version, $xml);

  // Get the entity ID of the project.
  $result = samurai_get_project_entity_id($machine_name, $core, $major_version);

  // -- Load the entity
  // -- Update the entity fields:
  // ---- latest_secure_minor_version
  // ---- latest_minor_version
  $project_entity = project_load($result);
  $project_entity->latest_secure_minor_version = $latest_secure_version;
  $project_entity->latest_minor_version = $latest_version;
  $project_entity->update_next_check = strtotime('+24 hours');
  $project_entity = entity_metadata_wrapper('project', $project_entity);

  // Save the entity.
  $project_entity->save();
}

/**
 * Retrieve the update data from a site site.
 *
 * @param string $site_url The site URL of a site.
 *
 * @return string $content The contents of the request.
 */
function samurai_get_site_update_data($site_url) {

  $content = samurai_curl_request($site_url . '/admin/reports/security_samurai');

  // Return the result.
  return $content;
}

/**
 * Returns an array of update data
 *
 * @param array[mixed] $decrypted_update_data Decrypted update data array.
 */
function samurai_process_update_data($decrypted_update_data) {

  // This array will be used to produce a tidy version of site_update_data.
  $projects_array = array();

  $modules = $decrypted_update_data['modules'];
  $themes = $decrypted_update_data['themes'];
  $core = $decrypted_update_data['core'];

  // Process the core section.
  if (!empty($core)) {
    // -- Humam readable name.
    // -- Machine name.
    // -- Split the version number into minor and major versions.
    // -- Compatible core version.
    // -- Define the project type as core.
    $c_name = 'Drupal core';
    $c_machine_name = 'drupal';
    $c_split_version = samurai_split_version_number($core['version']);
    $c_core = $c_split_version['major'] . '.x';
    $c_type = 'core';

    // Don't retrieve project information if the project is already in the
    // database.
    if (!samurai_project_exists($c_machine_name, $c_core, $c_split_version['major'])) {

      // Request the update history XML.
      $c_xml = samurai_xml_curl_request(
        SAMURAI_ORG_UPDATE_URL . $c_machine_name . '/' . $c_core);

      // -- Retrieve the latest secure minor version of core.
      // -- Retrieve the latest minor version of core.
      // -- Check if the core version is still supported.
      $c_latest_minor_secure = samurai_get_project_latest_secure_version(
        $c_machine_name,
        $c_core,
        $c_split_version['major'],
        $c_xml);
      $c_latest_minor = samurai_get_project_latest_version(
        $c_machine_name,
        $c_core,
        $c_split_version['major'],
        $c_xml);
      $c_supported = samurai_get_project_is_supported(
        $c_machine_name,
        $c_core,
        $c_split_version['major'],
        $c_xml);

      // Add the project to the database.
      samurai_add_project(
        $c_name,
        $c_machine_name,
        $c_type,
        $c_core,
        $c_split_version['major'],
        $c_latest_minor,
        $c_latest_minor_secure,
        $c_supported
      );
    }

    // Add the core project to the projects array.
    $projects_array[$c_machine_name] = array(
      'name' => $c_name,
      'machine_name' => $c_machine_name,
      'version' => $core['version'],
    );

  }

  // Process module projects.
  if (!empty($modules)) {
    foreach ($modules as $module => $m) {

      // -- Human readable name
      // -- Machine name
      // -- Compatible core version
      // -- Split the version number into the minor and major versions.
      $m_name = $m['name'];
      $m_machine_name = $m['machine_name'];
      $m_core = $m['core'];
      $m_split_version = samurai_split_version_number($m['version']);

      // Don't retrieve project information if the project is already in the
      // database.
      if (!samurai_project_exists($m_machine_name, $m_core, $m_split_version['major'])) {

        // Request the update history XML.
        $m_xml = samurai_xml_curl_request(
          SAMURAI_ORG_UPDATE_URL . $m_machine_name . '/' . $m_core);

        // -- Get the module supported status.
        // -- Get the module type e.g. contrib or custom.
        $m_status = samurai_get_project_is_supported(
          $m_machine_name,
          $m_core,
          $m_split_version['major'],
          $m_xml);
        $m_type = 'module' . samurai_get_project_type(
          $m_machine_name,
          $m_core,
          $m_split_version['major'],
          $m_xml);

        // Assign NULL values to version checks.
        $m_latest_minor = $m_split_version['minor'];
        $m_latest_minor_secure = $m_split_version['minor'];

        // First check that the project isn't custom.
        if ($m_type != 'module_custom') {

          // -- Retrieve the latest secure minor version of the module.
          // -- Retrieve the latest minor version of the module.
          $m_latest_minor_secure = samurai_get_project_latest_secure_version(
            $m_machine_name,
            $m_core,
            $m_split_version['major'],
            $m_xml);
          $m_latest_minor = samurai_get_project_latest_version(
            $m_machine_name,
            $m_core,
            $m_split_version['major'],
            $m_xml);
        }

        // Add the project to the database.
        samurai_add_project(
          $m_name,
          $m_machine_name,
          $m_type,
          $m_core,
          $m_split_version['major'],
          $m_latest_minor,
          $m_latest_minor_secure,
          $m_status
        );
      }

      // Add the module on to the projects_array.
      $projects_array[$m['machine_name']] = $m;
    }
  }

  // Process theme projects.
  if (!empty($themes)) {
    foreach ($themes as $theme => $t) {

      // -- Human readable name.
      // -- Machine name.
      // -- Compatible core version.
      // -- Split the version number into the minor and major versions.
      $t_name = $t['name'];
      $t_machine_name = $t['machine_name'];
      $t_core = $t['core'];
      $t_split_version = samurai_split_version_number($t['version']);

      // Don't retrieve project information if the project is already in the
      // database.
      if (!samurai_project_exists($t_machine_name, $t_core, $t_split_version['major'])) {

        // Request the update history XML.
        $t_xml = samurai_xml_curl_request(
          SAMURAI_ORG_UPDATE_URL . $t_machine_name  . '/' . $t_core);

        // -- Get the theme type e.g. contrib or custom.
        // -- Get the theme supported status.
        $t_type = 'theme' . samurai_get_project_type(
          $t_machine_name,
          $t_core,
          $t_split_version['major'],
          $t_xml);
        $t_status = samurai_get_project_is_supported(
          $t_machine_name,
          $t_core,
          $t_split_version['major'],
          $t_xml);

        // Assign NULL values to version checks.
        $t_latest_minor = $t_split_version['minor'];
        $t_latest_minor_secure = NULL;

        // Check that the project isn't custom.
        if ($t_type != 'theme_custom') {

          // -- Retrieve the latest secure minor version of the theme.
          // -- Retrieve the latest minor version of the theme
          $t_latest_minor_secure = samurai_get_project_latest_secure_version(
            $t_machine_name,
            $t_core,
            $t_split_version['major'],
            $t_xml);
          $t_latest_minor = samurai_get_project_latest_version(
            $t_machine_name,
            $t_core,
            $t_split_version['major'],
            $t_xml);
        }

        // Add the project to the database.
        samurai_add_project(
          $t_name,
          $t_machine_name,
          $t_type,
          $t_core,
          $t_split_version['major'],
          $t_latest_minor,
          $t_latest_minor_secure,
          $t_status
        );
      }

      // Add the theme on to the projects_array.
      $projects_array[$t['machine_name']] = $t;
    }
  }

  // Return the projects array for manipulation.
  return $projects_array;
}

/**
 * Returns an array of update data
 *
 * @param array[mixed] $unformatted_update_data Decrypted update data array.
 */
function samurai_process_drush_update_data($unformatted_update_data) {

  // The first entry is the header for the pm-list table
  // - Retrieve it and remove it from the array
  $first_entry = $unformatted_update_data[0];
  unset($unformatted_update_data[0]);

  // The string lengths of the headers will determine how long the strings
  // in the table data are
  // - Retrieve the start of each header
  $package_start = strpos($first_entry, 'Package');
  $name_start = strpos($first_entry, 'Name');
  $type_start = strpos($first_entry, 'Type');
  $status_start = strpos($first_entry, 'Status');
  $version_start = strpos($first_entry, 'Version');
  // - Retrieve the length of each header
  $package_length = $name_start - $package_start;
  $name_length = $type_start - $name_start;
  $type_length = $status_start - $type_start;
  $status_length = $version_start - $status_start;
  // The version does not need a $version_start due to it being the
  // last header item

  $update_data = array();

  // Loop through each entry in $unformatted_update_data
  foreach ($unformatted_update_data as $key => $data) {

    // Split the human readable name from the machine name
    $name = substr($data, $name_start, $name_length);
    $machine_name_start = strpos($name, '(');
    $machine_name_end = strpos($name, ')') - 1;
    $human_readable_length = ($machine_name_start - 1) - $name;
    $human_readable_name = substr($name, 0, $human_readable_length);
    $machine_name = substr($name, $machine_name_start + 1, $machine_name_end - $machine_name_start);

    // Get the package so Drupal core modules can be omitted
    $package = substr($data, $package_start, $package_length);
    $package = str_replace(' ', '', $package);

    // Get the version numbers
    $version = substr($data, $version_start);
    $version_splitted = samurai_split_version_number($version);
    $core_version = samurai_split_version_number_core($version);

    if ($package == 'Core') {
      $core_version = $version_splitted['major'] . '.x';
    }

    // Get the project status
    $status = substr($data, $status_start, $status_length);
    $status = str_replace(' ', '', $status);

    // Get the project type
    $type = substr($data, $type_start, $type_length);

    if (!empty($name) && $status == 'Enabled') {
      if ($package == 'Core') {
        if (!isset($update_data['drupal'])) {
          $update_data['drupal']['name'] = 'Drupal core';
          $update_data['drupal']['machine_name'] = 'drupal';
          $update_data['drupal']['version'] = $version;
          $human_readable_name = 'Drupal core';
          $machine_name = 'drupal';
        }
      } else {
        $update_data[$machine_name]['name'] = $human_readable_name;
        $update_data[$machine_name]['machine_name'] = $machine_name;
        $update_data[$machine_name]['core'] = $core_version;
        $update_data[$machine_name]['version'] = $version;
      }

      if (!samurai_project_exists($machine_name, $core_version, $version_splitted['major'])) {

        $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core_version);

        $type = str_replace(' ', '', $type);
        $type = strtolower($type) . samurai_get_project_type(
          $machine_name,
          $core_version,
          $version_splitted['major'],
          $xml);

        if ($package == 'Core') {
          $type == 'core';
        }

        // -- Retrieve the latest secure minor version of core.
        // -- Retrieve the latest minor version of core.
        // -- Check if the core version is still supported.
        $latest_minor_secure = samurai_get_project_latest_secure_version(
          $machine_name,
          $core_version,
          $version_splitted['major'],
          $xml);
        $latest_minor = samurai_get_project_latest_version(
          $machine_name,
          $core_version,
          $version_splitted['major'],
          $xml);
        $supported = samurai_get_project_is_supported(
          $machine_name,
          $core_version,
          $version_splitted['major'],
          $xml);

        // Add the project to the database
        samurai_add_project(
          $human_readable_name,
          $machine_name,
          $type,
          $core_version,
          $version_splitted['major'],
          $latest_minor,
          $latest_minor_secure,
          $supported
        );
      }
    }
  }

  return $update_data;
}

/**
 * Gets a project type e.g. Contrib or Custom
 *
 * @param string $machine_name The machine name of the project.
 * @param string $core The supported core version e.g. 7.x OR 6.x
 * @param stirng $major_version The major version of the project e.g. 1.x
 * @param SimpleXMLElement[object] $xml The XML of the project.
 *
 * @return string The type of project e.g. '_custom' or '_contrib'.
 */
function samurai_get_project_type($machine_name, $core, $major_version, $xml = NULL) {

  // Check if the project XML has been passed.
  if (is_null($xml)) {
    $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core);
  }

  // Check if the XML title is set, this is usually a giveaway as to whether
  // the project exists.
  if (!isset($xml->title)) {
    // Project is custom.
    $return = '_custom';
  } else {
    // Project is contrib.
    $return = '_contrib';
  }
  return $return;
}

/**
 * Checks whether a project major version is supported.
 *
 * @param string $machine_name The machine name of the project.
 * @param string $core The supported core version e.g. 7.x
 * @param string $major_version The major version of the project e.g. 1.x
 * @param SimpleXMLElement[object] $xml The XML of the project.
 *
 * @return int 1/0 Is the project supported? 1 = TRUE, 0 = FALSE
 */
function samurai_get_project_is_supported($machine_name, $core, $major_version, $xml = NULL) {

  // Check if the project XML has been passed.
  if (is_null($xml)) {
    $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core);
  }

  // -- Check the project status from the xml.
  // -- Split the supported majors if there is more than one supported.
  $supported_majors = (string)$xml->supported_majors;
  $supported_status = (string)$xml->project_status;
  $supported_majors_split = explode(',', $supported_majors);

  // Loop through each major, comparing it to the major provided in the params.
  $supported = 0;
  foreach ($supported_majors_split as $major) {
    if ($major == $major_version) {
      $supported++;
    }
  }

  // If $supported is greater than 0
  // then return TRUE, else, FALSE.
  if ($supported_status != 'unsupported' && $supported > 0) {
    return 1;
  } else {
    return 0;
  }
}

/**
 * Retrieves the latest available version of a contrib project.
 *
 * @param string $machine_name The machine name of the project.
 * @param string $core The compatible core version e.g. 7.x
 * @param string $major_version The major version of the project e.g. 1.x
 * @param SimpleXMLElement[object] $xml THe XML of the project.
 *
 * @return string $return The latest available minor version.
 */
function samurai_get_project_latest_version($machine_name, $core, $major_version, $xml = NULL) {

  // Check if the project XML has been passed.
  if (is_null($xml)) {
    $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core);
  }

  // Loop through each release entry looking for the corresponding major version
  // with the latest minor version.
  $return = '';
  foreach ($xml->releases as $release => $rel) {
    foreach ($rel as $key => $r) {
      if ($major_version == (string)$r->version_major) {

        // -- A match has been found for the latest major version.
        // -- Break out of the foreach loop.
        $return = (string)$r->version_patch;
        break;
      }
    }
  }

  // Return the latest project version.
  return $return;
}

/**
 * Retrieves the latest available secure version of a contrib project.
 *
 * @param string $machine_name The machine name of the project.
 * @param string $core The compatible core version e.g. 7.x
 * @param string $major_version The major version of the project e.g. 1.x
 * @param SimpleXMLElement[object] $xml The XML of the object.
 *
 * @return string $return The latest available secure minor version
 */
function samurai_get_project_latest_secure_version($machine_name, $core, $major_version = NULL, $xml = NULL) {

  // Check if the project XML has been passed.
  if (is_null($xml)) {
    $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core);
  }

  variable_set('samurai_debug', json_encode($xml));

  // Set the scope of the return variable.
  $return = '';

  // If major version parameter is set.
  if (!is_null($major_version)) {

    // Loop through each release entry looking for the corresponding major
    // version with the term 'security update'
    foreach ($xml->releases as $release => $rel) {
      foreach ($rel as $key => $r) {
        if ($major_version == (string)$r->version_major) {
          // The terms are often not ordered the same.
          // Check each term entry.
          if (!empty($r->terms)) {
            foreach ($r->terms->term as $term => $t) {

              $term_value = (string)$t->value;
              if ($term_value == 'Security update') {

                // Latest secure version has been found
                $return = (string)$r->version_patch;

                // Break out of the foreach loop.
                break;
              }
            }
          }
          // Break out of the foreach loop.
          if (strlen($return) > 0) {
            break;
          }
        }
      }
    }
  } else {
    // Query the database for all of the possible version_majors
    $result = db_query("SELECT major_version FROM {securitysamurai_projects}
      WHERE machine_name = :machine_name
      AND core = :core", array(
        ':machine_name' => $machine_name,
        ':core' => $core
      ));

    $return = array();

    // Check result isn't empty.
    if (!empty($result)) {
      foreach ($result as $key => $value) {

        // Loop through each release entry looking for the corresponding major
        // version with the term 'security update'
        foreach ($xml->releases->release as $release => $rel) {
          foreach ($rel as $key => $r) {
            if ($value == (string)$r->major_version) {
              // The terms are oftem not ordered the same.
              // Check each term entry.
              if (!empty($r->terms)) {
                foreach ($r->terms->term as $term => $t) {

                  if ($term == 'Security update') {

                    // The latest secure version has been found.
                    $return[(string)$r->major_version] = (string)$r->version_patch;

                    // Break out of the foreach loop.
                    break;
                  }
                }
              }
              // Break out of the foreach loop.
              if (strlen($return) > 0) {
                break;
              }
            }
          }
        }
      }
    } else {
      // The result is empty, which means the project isn't in the database.
      return NULL;
    }
  }
  // Return the latest secure minor version
  return $return;
}
