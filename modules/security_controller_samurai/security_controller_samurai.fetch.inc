<?php
/**
 * @file
 * functions to retrieve available update data for the modules and themes
 *
 * @author jkswoods
 */

/**
 * @todo maybe combine $modules and $themes in @see _samurai_project_update_data()
 */

/**
 * Implements hook_cron
 */
function security_controller_samurai_cron() {
  // Get an array of clients that have not yet had their sites checked/need to be checked.
  $result = db_query("SELECT id FROM {securitysamurai_client}
    WHERE update_next_check <= 0 OR
    update_next_check <= :time",
    array(
      ':time' => REQUEST_TIME,
  ));

  // Loop through each result, obtaining the site_update_data and processing it.
  foreach ($result as $record) {
    // Load the client.
    $client = client_load($record->id);
    // Get the update data from the client's site.
    $encoded_update_data = samurai_get_site_update_data($client->site_url);
    // Decode the json encoded update data, so we can directly access the encrypted string.
    $encrypted_update_data = drupal_json_decode($encoded_update_data);
    // Decrypt the update data.
    $decrypted_update_data = drupal_json_decode(trim(samurai_decrypt_update_data($encrypted_update_data['encrypted'], $client->encryption_key)));
    // Process the data and make final changes to the entity.
    $client->site_update_data = drupal_json_encode(_samurai_process_update_data($decrypted_update_data));
    $client->update_next_check = strtotime('+24 hours');
    // Save the entity.
    entity_save('client', $client);
    // Log to watchdog.
    watchdog('Security samurai', t('@name has been scanned.', array('@name' => $client->name)));
  }
  // Security updates will be checked for every single cron run.
  // Checking for security updates is the fairly quick and simple, it can be done in 2 curl requests.
  // Get the contrib projects security announcements.
  $xml_contrib = samurai_xml_curl_request(SAMURAI_ORG_SA_CONTRIB);
  // Check $xml_contrib isn't null before continuing.
  if (!is_null($xml_contrib)) {
    // Process the contrib projects security announcements.
    _samurai_process_sa($xml_contrib, 'contrib');
  }
  // Get the core projects security announcements.
  $xml_core = samurai_xml_curl_request(SAMURAI_ORG_SA_CORE);
  // Check $xml_core isn't null before continuing
  if (!is_null($xml_core)) {
    // Process the core projects security announements.
    _samurai_process_sa($xml_core, 'core');
  }
  // Check for general updates of contrib projects
  // Check every ten minutes, check 10 projects every 10 minutes
  $check = variable_get('security_controller_samurai_contrib_check', NULL);
  $offset = variable_get('security_controller_samurai_contrib_check_offset', NULL);
  if ((!is_null($check) && $check <= REQUEST_TIME) && !is_null($offset)) {
    // Now is the time to check for normal module updates.
    // To save resources, maybe we should limit the checking to only a certain number of modules?
    /**
     * @todo
     */
  } else {
    // $check might be null
    // $offset might be null
    // OR it isn't time to check yet.
    /**
     * @todo
     */
  }
}

/**
 * Process the security announcment feed
 *
 * @param SimpleXMLElement[object] The sa feed.
 * @param string $type core or contrib.
 *
 * @todo project_name should be the machinename of the project.
 */
function _samurai_process_sa($xml, $type) {
  // Loop through each item in the sa $xml
  foreach ($xml->channel->item as $item) {
    // Remove the tags each value. We have to do this because sometimes
    // RSS doesn't validate as SimpleXML.
    foreach ($item as $key => $value) {
      $item->$key = strip_tags($value);
    }
    // The description contains everything needed and will need to be split up.
    $description = (string)$item->description;
    // Get the Security announcement ID from the description.
    $sa = samurai_split_sa_data($description, 'Advisory ID:', 'Project:');
    // Check to make sure the SA hasn't already been added to the database before continuing.
    if (!samurai_sa_exists($sa)) {
      // SA doesn't exist, we can continue to obtain the other required database fields.
      if ($type == 'contrib') {
        // If the XML is for contrib projects.
        $project_name = samurai_split_sa_data($description, 'Project:', '(third-party module)');
        // Now we have the project name, let's get the machine name.
        $machine_name;
      } else {
        // If the XML is for core.
        $project_name = 'Drupal core';
      }
      // Get the versions affected.
      $version_aff_raw = samurai_split_sa_data($description, 'Version:', 'Date:');
      // Process the version affected value to be a json array.
      $version_aff_json = drupal_json_encode(explode(',', $version_aff_raw));
      $version_aff = str_replace(' ', '', $version_aff_json);
      // Get the risk level.
      $risk_level = samurai_split_sa_data($description, 'Security risk:', 'Vulnerability:');
      // Get the time the announcement was made.
      $time = strtotime(samurai_split_sa_data($description, 'Date:', 'Security risk:'));
      // Get the vulnerability description.
      $vulnerability = samurai_split_sa_data($description, 'Vulnerability:', 'CVE identifier(s)');
      // Get the solution to fix the $vulnerabilty.
      $solution  = samurai_split_sa_data($description, 'Solution', 'Reported by');
      // Create a new SA entity
      $security_announcement = entity_create('security_announcement', array(
        'sa_id' => $sa,
        'risk_level' => $risk_level,
        'date' => $time,
        'version' => $version_aff,
        'vulnerability' => $vulnerability,
        'solution' => $solution,
        'project_name' => $project_name
      ));
      // Save the newly created security announcement
      $security_announcement_wrapper = entity_metadata_wrapper('security_announcement', $security_announcement);
      $security_announcement_wrapper->save();
      watchdog('Security samurai', $sa . t(' New security update data'));
    } else {
      continue;
    }
  }
}

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
  // Set the destination URL to retrieve the update data.
  curl_setopt($curl, CURLOPT_URL, $url);
  // We want to retrieve content from the page.
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
    // SimpleXMLElement will handle any errors itself.
    // So we can just return NULL here.
    return;
  }
  // The XML object was created successfully.
  // Return the SimpleXMLELement object.
  return $xml;
}

/**
 * Check that samurai has access to the client site.
 *
 * @todo
 */
function _samurai_check_site_access($site_url) {
  // Not implemented
}

/**
 * Retrieve the update data from a client site.
 *
 * @param string $site_url The site URL of a client.
 *
 * @return string $content The contents of the request.
 */
function samurai_get_site_update_data($site_url) {
  // Create a new curl object
  $content = samurai_curl_request($site_url . '/admin/reports/security_samurai');
  // Return the result.
  return $content;
}

/**
 * Add the update data to the securitysamurai_projects table.
 *
 * @param array[mixed] $decrypted_update_data Decrypted update data array.
 *
 * @todo Reduce the amount of code and do the modules and themes together.
 */
function _samurai_process_update_data($decrypted_update_data) {
  // This array will be used to produce a tidy version of site_update_data
  $projects_array = array();

  // Loop through the modules, themes and core version.
  $modules = $decrypted_update_data['modules'];
  $themes = $decrypted_update_data['themes'];
  $core = $decrypted_update_data['core'];
  // Process module projects
  if (!empty($modules)) {
    foreach ($modules as $module => $m) {
      // Human readable name
      $m_name = $m['name'];
      // Machine name
      $m_machine_name = $m['machine_name'];
      // Compatible core version
      $m_core = $m['core'];
      // Split the version number into the minor and major versions.
      $m_split_version = samurai_split_version_number($m['version']);
      // Save time by only doing the functions that are required if the project isn't already in the database.
      if (!samurai_project_exists($m_machine_name, $m_core, $m_split_version['major'])) {
        // Create the XML object
        $m_xml = samurai_xml_curl_request(
          SAMURAI_ORG_UPDATE_URL . $m_machine_name . '/' . $m_core);
        // Get the module supported status
        $m_status = samurai_get_project_is_supported(
          $m_machine_name,
          $m_core,
          $m_split_version['major'],
          $m_xml);
        // Get the module type e.g. contrib or custom
        $m_type = 'module' . samurai_get_project_type(
          $m_machine_name,
          $m_core,
          $m_split_version['major'],
          $m_xml);
        // Assign NULL values to version checks.
        $m_latest_minor = $m_split_version['minor'];
        $m_latest_minor_secure = $m_split_version['minor'];
        // First check that the project isn't custom.
        if ($m_type_status['type'] != '_custom') {
          // Retrieve the latest secure minor version of the module.
          $m_latest_minor_secure = samurai_get_project_latest_secure_version(
            $m_machine_name,
            $m_core,
            $m_split_version['major'],
            $m_xml);
          // Retrieve the latest minor version of the module in correl with the major_version.
          $m_latest_minor = samurai_get_project_latest_version(
            $m_machine_name,
            $m_core,
            $m_split_version['major'],
            $m_xml);
        }
        // Add the project.
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
      // Add the module on to the projects_array
      $projects_array[$m['machine_name']] = $m;
    }
  }
  // Process theme projects
  if (!empty($themes)) {
    foreach ($themes as $theme => $t) {
      // Human readable name
      $t_name = $t['name'];
      // Machine name
      $t_machine_name = $t['machine_name'];
      // Compatible core version
      $t_core = $t['core'];
      // Split the version number into the minor and major versions.
      $t_split_version = samurai_split_version_number($t['version']);

      // Save time by only doing the functions that are required if the project isn't already in the database.
      if (!samurai_project_exists($t_machine_name, $t_core, $t_split_version['major'])) {
        // Create the XML object.
        $t_xml = samurai_xml_curl_request(
          SAMURAI_ORG_UPDATE_URL . $t_machine_name  . '/' . $t_core);
        // Get the theme type e.g. contrib or custom
        $t_type = 'theme' . samurai_get_project_type(
          $t_machine_name,
          $t_core,
          $t_split_version['major'],
          $t_xml);
        // Get the theme supported status
        $t_status = samurai_get_project_is_supported(
          $t_machine_name,
          $t_core,
          $t_split_version['major'],
          $t_xml);
        // Assign NULL values to version checks.
        $t_latest_minor = $t_split_version['minor'];
        $t_latest_minor_secure = NULL;
        // Frist check that the project isn't custom.
        if ($t_type_status['type'] != '_custom') {
          // Retrieve the latest secure minor version of the theme.
          $t_latest_minor_secure = samurai_get_project_latest_secure_version(
            $t_machine_name,
            $t_core,
            $t_split_version['major'],
            $t_xml);
          // Retrieve the latest minor version of the theme
          $t_latest_minor = samurai_get_project_latest_version(
            $t_machine_name,
            $t_core,
            $t_split_version['major'],
            $t_xml);
        }
        // Add the project.
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

      // Add the theme on to the projects_array
      $projects_array[$t['machine_name']] = $t;
    }
  }
  // Process core
  if (!empty($core)) {
    // Humam readable name
    $c_name = 'Drupal core';
    // Machine name
    $c_machine_name = 'drupal';
    // Split the version number into minor and major versions.
    $c_split_version = samurai_split_version_number($core['version']);
    // Compatible core version
    $c_core = $c_split_version['major'] . '.x';
    // Define the project type as core.
    $c_type = 'core';

    // Save time by only doing the functions that are required if the project isn't already in the database.
    if (!samurai_project_exists($c_machine_name, $c_core, $c_split_version['major'])) {
      // Create the XML object.
      $c_xml = samurai_xml_curl_request(
        SAMURAI_ORG_UPDATE_URL . $c_machine_name . '/' . $c_core);
      // Retrieve the latest secure minor version of core.
      $c_latest_minor_secure = samurai_get_project_latest_secure_version(
        $c_machine_name,
        $c_core,
        $c_split_version['major'],
        $c_xml);
      // Retrieve the latest minor version of core.
      $c_latest_minor = samurai_get_project_latest_version(
        $c_machine_name,
        $c_core,
        $c_split_version['major'],
        $c_xml);
      // Check if the core version is still supported.
      $c_supported = samurai_get_project_is_supported(
        $c_machine_name,
        $c_core,
        $c_split_version['major'],
        $c_xml);
      // Add the project.
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
    // Add the core project to the projects array
    $projects_array[$c_machine_name] = array(
      'name' => $c_name,
      'machine_name' => $c_machine_name,
      'version' => $core['version'],
    );
  }
  // Return the projects array for manipulation
  return $projects_array;
}

/**
 * Gets a project type e.g. Contrib or Custom
 *
 * @param string $machine_name The machine name of the project.
 * @param string $core The supported core version e.g. 7.x
 * @param string $major_version The major version of the project e.g. 1.x
 *
 * @todo Retrieving the type and status should be kept separate.
 */
function _samurai_get_project_type_status($machine_name, $core, $major_version, $xml = NULL) {
  // Define the return array
  $return = array();

  if (is_null($xml)) {
    // The XML has not been passed.
      $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core);
  }

  if (!isset($xml->title)) {
    // The XML title isn't set. Presume custom module.
    $return['type'] = '_custom';
    // Project is custom, status should  be NULL.
    $return['status'] = NULL;
  } else {
    // Project is available on Drupal.org
    // Return it as contrib.
    $return['type'] = '_contrib';
    // Check that the major version is supported.
    $return['status'] = samurai_get_project_is_supported($machine_name, $core, $major_version, $xml);
  }
  // Return the type and status.
  return $return;
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
    // The XML parameter has not been passed.
    $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core);
  }
  // Check if the XML title is set, this is usually a giveaway as to whether
  // the project exists.
  if (!isset($xml->title)) {
    // The title of the project isn't set, so it must be a custom project.
    $return = '_custom';
  } else {
    // The title of the project is set, so it must be a contributed project.
    $return = '_contrib';
  }
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
  if (is_null($xml)) {
    // The XML has not been passed.
    $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core);
  }
  // Check the project status from the xml.
  $supported_majors = (string)$xml->supported_majors;
  $supported_status = (string)$xml->project_status;
  // Split the supported majors if there is more than one supported.
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
  if ($supported_status != 'unsupported' || $supported > 0) {
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
  if (is_null($xml)) {
    // The XML has not been passed.
    $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core);
  }
  // Loop through each release entry looking for the corresponding major version
  // with the latest minor version.
  $return = '';
  foreach ($xml->releases->release as $release => $r) {
    // As Drupal.org lists the updates by most recent, we can loop
    // through efficiently and easily.
    if ($major_version == (string)$r->version_major) {
      // We've found a match for the latest major version.
      // Now to set the latest minor version variable.
      $return = (string)$r->version_patch;
      // Break out of the foreach loop.
      break;
    }
  }
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
function samurai_get_project_latest_secure_version($machine_name, $core, $major_version, $xml = NULL) {
  if (is_null($xml)) {
    // The XML has not been passed.
    $xml = samurai_xml_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core);
  }
  // Loop through each release entry looking for the corresponding major version
  // with the term 'Security update'
  $return = '';
  foreach ($xml->releases->release as $release => $r) {
    // As Drupa.org lists the updates by most recent, we can loop
    // through efficiently and easily.
    if ($major_version == (string)$r->version_major) {
      // The terms are often not ordered the same.
      // We will have to check each term entry.
      if (!empty($r->terms)) {
        foreach ($r->terms->term as $term => $t) {
          $term_value = (string)$t->value;
          if ($term_value == 'Security update') {
            // We have found the latest secure version for the specified major.
            $return = (string)$r->version_patch;
            // Break out of the inner foreach loop;
            break;
          }
        }
      }
      if (strlen($return) > 0) {
        // Break out of the foreach loop.
        break;
      }
    }
  }
  return $return;
}
