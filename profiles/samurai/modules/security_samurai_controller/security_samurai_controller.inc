<?php
/**
 * @file
 * General helper functions that have no specific place.
 *
 * @author jkswoods
 */

/**
 * Retrieve an array of sites affected by a particular security announcement.
 *
 * @param string $sa_id The security announcement ID.
 * @param string $project_name The name of the affected project.
 * @param string $core The json encoded string of compatible core version/s.
 *
 * @return string $return HTML content to return.
 * @return NULL Returned when there is no result from the query.
 */
function samurai_get_affected_sites($sa_id, $project_name, $core) {

  // Strip whitespace from project_name
  $project_name = trim($project_name);

  // Retrieve the data on the site if they have the project in the update_data
  if (samurai_sa_exists($sa_id)) {

    // Run the query.
    $results = db_query('SELECT * FROM {securitysamurai_site} WHERE
      LOCATE(:project_name, site_update_data) > 0',
      array(
        ':project_name' => $project_name
      ))->fetchAll();

    // Decode the JSON into a PHP array.
    $core_array = drupal_json_decode($core);

    if (!empty($results)) {

      $return = '';
      foreach ($results as $result => $r) {

        // - Check that the core versions are matching.
        // - Split the version number of Drupal core into major and minor.
        // - Retrieve the machine name of the affected project.
        // - Split the version number of the project into major and minor.
        $site_update = drupal_json_decode($r->site_update_data);
        $split_version_core = samurai_split_version_number($site_update['drupal']['version']);
        $machine_name = samurai_project_machine_name($project_name);
        $split_version_contrib = samurai_split_version_number($site_update[$machine_name]['version']);

        // Loop through each affected core version.
        foreach ($core_array as $core) {

          // Compare the core versions.
          if ($core == $split_version_core['major'] . '.x') {

            // Check that the site minor version is less than the secure version.
            if ($split_version_contrib['minor'] <
              samurai_project_secure_minor($machine_name, $core, $split_version_contrib['major'])) {

              // If the core versions are correct, return the site markup.
              $return .= '<p class="content"><a href="/client-site/' . $r->id . '">' . $r->name . '</a></p>';
            }
          }
        }
      }

      if (!empty($return)) {

        // Return the result.
        return $return;
      } else {

        return NULL;
      }
    } else {

      // No result. return NULL.
      return NULL;
    }
  }
}


/**
 * Retrieve the risk category e.g. Low, moderate... etc.
 *
 * returns moderate, critical or low
 *
 * @param string $risk_raw The raw views output of the risk field.
 *
 * @return string $return The risk level.
 */
function samurai_parse_risk($risk_raw) {

  // - Get the starting position of the risk classification.
  // - Get the finishing position of the risk classification.
  // - Calculate the length of the risk classification.
  // - Retrieve the risk classification.
  // - Remove spaces from the result.
  $risk_pos_start = strpos($risk_raw, '(') + 1;
  $risk_pos_end = strpos($risk_raw, ')');
  $risk_length = $risk_pos_end - $risk_pos_start;
  $risk = substr($risk_raw, $risk_pos_start, $risk_length);
  $return = strtolower(str_replace(' ', '', $risk));

  // Return the result.
  return $return;
}

/**
 * Check if a site exists in the database.
 *
 * @param string $site_name The name of the site.
 *
 * @return bool TRUE/FALSE The site exists in the database?
 */
function samurai_site_exists($site_name) {

  // Query the database.
  $result = db_query('SELECT id FROM {securitysamurai_site} WHERE
    name = :name',
    array(
      ':name' => $site_name,
    ))->fetchField();

  if (!empty($result)) {

    // The result isn't empty.
    // Site exists. Return TRUE.
    return TRUE;
  } else {

    // The result is empty.
    // Site doesn't exist. Return FALSE.
    return FALSE;
  }
}

/**
 * Check if a SA exists in the database.
 *
 * @param string $sa_id
 *
 * @return bool TRUE/FALSE The SA exists in the database.
 */
function samurai_sa_exists($sa_id) {

  // SA must be less that 30chars
  if (strlen($sa_id) <= 30) {

    // Query the database.
    $result = db_query('SELECT id FROM {securitysamurai_sa} WHERE
      sa_id = :sa_id',
      array(
        ':sa_id' => $sa_id,
      ))->fetchField();
  } else {

    return TRUE;
  }

  if (!empty($result)) {

    // The result isn't empty.
    // SA exists. Return TRUE.
    return TRUE;
  } else {

    // The result is empty.
    // SA doesn't exist. Return FALSE.
    return FALSE;
  }
}

/**
 * Checks whether a project is already in the database.
 *
 * @param string $machine_name The machine name of the project.
 * @param string $core The compatible core version e.g. 7.x, 6.x, 8.x
 * @param string $major_version The major version number e.g. 1.X
 *
 * @return bool TRUE/FALSE Depends on the project existing in the database
 */
function samurai_project_exists($machine_name, $core, $major_version) {

  // Query the database for the project.
  $result = db_query('SELECT name FROM {securitysamurai_projects} WHERE
    machine_name = :machine_name AND
    core = :core AND
    major_version = :major_version',
    array(
      ':machine_name' => $machine_name,
      ':core' => $core,
      ':major_version' => $major_version
    )
  )->fetchField();

  if (!empty($result)) {

    // The project is in the database.
    return TRUE;
  } else {

    // The project is not in the database.
    return FALSE;
  }
}

/**
 * Returns a array of available project major versions.
 *
 * @param string $machine_name The machine name of the project.
 * @param string $core The compatible core version e.g. 7.x
 *
 * @return array[int] $return The array of major versions.
 */
function samurai_get_available_major_versions($machine_name, $core) {

  // Query the database for the major_versions
  $result = db_query('SELECT major_version FROM {securitysamurai_projects} WHERE
    machine_name = :machine_name AND
    core = :core', array(
      ':machine_name' => $machine_name,
      ':core' => $core
    ))->fetchAll();

  // Check if the result is empty.
  if (!empty($result)) {
    $return = array();
    foreach ($result as $res => $r) {

      $return[sizeof($return)] = $r->major_version;
    }
    // Return the result.
    return $return;
  } else {

    // The result is empty.
    return NULL;
  }
}

/**
 * Returns the entity ID of a project.
 *
 * @param string $machine_name The machine name of the project
 * @param string $core The compatible core version e.g. 7.x
 * @param int $major_version The major_version
 */
function samurai_get_project_entity_id($machine_name, $core, $major_version) {

  // Query the database for the ID
  $result = db_query("SELECT id FROM {securitysamurai_projects} WHERE
    major_version = :major_version AND
    machine_name = :machine_name AND
    core = :core", array(
      ":major_version" => $major_version,
      ":machine_name" => $machine_name,
      ":core" => $core
    ))->fetchField();

  // Check if the result is empty.
  if (!empty($result)) {

    return $result;
  } else {

    return NULL;
  }
}

/**
 * Adds a project to the securitysamurai_projects table.
 *
 * @param string $name The human readable name of the project.
 * @param string $machine_name The machine name of the project.
 * @param string $type The project type.
 *        e.g.
 *          Theme Contrib,
 *          Theme Custom,
 *          Core,
 *          Module Contrib,
 *          Module Custom
 * @param string $major_version The major version of the project e.g. x.1
 * @param string $latest_minor_versino The minor version of the project 1.x
 * @param string $latest_secure_minor_version The latest secure minor version of the project e.g. 1.x
 * @param int $supported 1 OR 0, 1 for supported, 0 for unsupported.
 */
function samurai_add_project(
  $name,
  $machine_name,
  $type,
  $core,
  $major_version,
  $latest_minor_version,
  $latest_secure_minor_version,
  $supported = NULL) {

  if (!samurai_project_exists($machine_name, $core, $major_version)) {

    // The project has not yet been created.
    // Create the new project entity.
    $project = entity_create('project', array(
      'name' => $name,
      'machine_name' => $machine_name,
      'project_type' => $type,
      'major_version' => $major_version,
      'latest_minor_version' => (int)$latest_minor_version,
      'latest_secure_minor_version' => (int)$latest_secure_minor_version,
      'core' => $core,
      'supported' => $supported,
      'update_next_check' => strtotime("+24 hours")
    ));

    // - Save the entity.
    // - Log a message to watchdog.
    $project_wrapper = entity_metadata_wrapper('project', $project);
    $project_wrapper->save();
    watchdog('Security samurai', $name . t(' has been added to the project list'));
  }
}

/**
 * Split the XML data, to retrieve specific values.
 *
 * This will retrieve the string in between $str1 and $str2.
 *
 * @param string $xml_string The whole string.
 * @param int $str1 The first string.
 * @param int $str2 The second string.
 *
 * @return string $return The value of the collected string
 */
function samurai_split_sa_data($xml_string, $str1, $str2 = NULL) {

  if (is_null($str2)) {

    // - If $str2 is null then it can't be used.
    // - Work out the starting position of the string to return.
    // - Retrieve the returning value.
    $string1_start = strpos($xml_string, $str1);
    $return_start = $string1_start + (strlen($str1) + 1);
    $return = substr($xml_string, $return_start);
  } else {
    // - Get the start position of the two specified strings.
    // - Work out the starting position of the string to return.
    // - Work out the length of the string to return.
    // - Retrieve the returning value.
    $string1_start = strpos($xml_string, $str1);
    $string2_start = strpos($xml_string, $str2);
    $return_start = $string1_start + (strlen($str1) + 1);
    $return_length = $string2_start - $return_start - 1;
    $return = substr($xml_string, $return_start, $return_length);
  }

  // Return $return.
  return $return;
}

/**
 * Decrypt the update data retrieved from a site site.
 *
 * @param string $update_data The encrypted update data string.
 * @param string $encryption_key The encryption key.
 *
 * @return string $decrypted The decrypted update data.
 */
function samurai_decrypt_update_data($update_data, $encryption_key) {

  // - Create the SHA key from the encryption key.
  // - Base 64 decode the update data.
  // - Get the IV from $ciphertext_dec.
  // - Remove the IV from $ciphertext_dec.
  // - Decrypt $ciphertext_dec.
  // - There can be problems with \u0022 appearing.
  $key = hash('SHA256', $encryption_key, TRUE);
  $ciphertext_dec = base64_decode($update_data);
  $iv_size = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_CBC);
  $iv_dec = substr($ciphertext_dec, 0, $iv_size);
  $ciphertext_dec = substr($ciphertext_dec, $iv_size);
  $plaintext_dec = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, $ciphertext_dec, MCRYPT_MODE_CBC, $iv_dec);
  $decrypted = str_replace('\u0022', '"', $plaintext_dec);

  // Return the decrypted json string
  return $decrypted;
}

/**
 * Change the project type
 * e.g. From Contrib -> Custom
 *
 * @param string $project_name The project name.
 * @param string $type The new project type.
 * @param string $core The compatible core version
 * @param string $major_version The major version number e.g. x.1
 */
function samurai_mark_project_type($project_name, $core, $major_version, $type) {}

/**
 * Splits a version number into their major and minor numbers.
 *
 * @param string $version_number The unsplit version number e.g. 1.14.
 *
 * @return array[mixed] $split The split version number.
 */
function samurai_split_version_number($version_number) {

  $return = array();
  if (strpos($version_number, '7.x-') !== FALSE) {

    // Cater for version numbers that contain 7.x-
    $version_number = substr($version_number, 4);
  }
  if (strpos($version_number, '-') !== FALSE) {

    // Remove the ending version e.g. -beta1
    $pos = strpos($version_number, '-');
    $version_number = substr($version_number, 0, $pos);
  }
  if (strpos($version_number, '+') !== FALSE) {

    // Remove the ending version e.g. +rc1
    $pos = strpos($version_number, '+');
    $version_number = substr($version_number, 0, $pos);
  }

  // Get the position of the splitter.
  $splitter = strpos($version_number, '.');
  $minor = substr($version_number, $splitter + 1);
  $major = substr($version_number, 0, $splitter);

  // Remove the spaces from the split version number
  $minor = str_replace(' ', '', $minor);
  $major = str_replace(' ', '', $major);

  // - Add the values to the return array.
  // - Return the array.
  $return['minor'] = $minor;
  $return['major'] = $major;
  return $return;
}

/**
 * Splits a drupal.org project version number returning the
 * compatible core version
 *
 * @param string $version_number The version number of the project e.g. 7.x-1.1
 */
function samurai_split_version_number_core($version_number) {

  // Retrieve the position of '-'
  $core_version_end = strpos($version_number, '-');

  // Return the core version number
  return substr($version_number, 0, $core_version_end);
}

/**
 * Retrieve the latest secure version as stored in the database.
 *
 * @param string $machine_name The machine name of the project.
 * @param string $core The compatible core version e.g. 7.x, 6.x
 * @param string $major_version The major version of the project.
 */
function samurai_project_secure_minor($machine_name, $core, $major_version) {

  if (samurai_project_exists($machine_name, $core, $major_version)) {

    $result = db_query('SELECT latest_secure_minor_version FROM {securitysamurai_projects} WHERE
      machine_name = :machine_name AND
      core = :core AND
      major_version = :major_version',
      array(
        ':machine_name' => $machine_name,
        ':core' => $core,
        ':major_version' => $major_version
      ))->fetchField();

    // Check if the result is empty.
    if (!empty($result)) {

      // The result is not empty.
      // Set the result to $return.
      $return = $result;

      // return the result.
      return $return;
    } else {

      // The result is empty
      return NULL;
    }
  }
}

/**
 * Retrieve the machine name for a project as stored in the database.
 *
 * @param string $project_name The project_name NOT the machine name
 */
function samurai_project_machine_name($project_name) {

  // Query the database for the machine_name
  $result = db_query("SELECT machine_name FROM {securitysamurai_projects} WHERE
    name = :project_name",
    array(
      ':project_name' => $project_name
    ))->fetchField();

  // Check if the result is empty.
  if (!empty($result)) {

    // The result is not empty.
    // Set the result to $return.
    $return = $result;

    // Return the result.
    return $return;
  } else {

    // The result is empty.
    return NULL;
  }
}

/**
 * Check that samurai has access to the site site.
 *
 * @param string $site_url The site url of which to check access on.
 */
function samurai_check_site_access($site_url) {

  // Now we must check that we have access to the site.
  $content = samurai_curl_request($site_url . '/admin/reports/security_samurai');

  // Check that the content returned is not empty.
  if (!empty($content)) {

    $decoded = drupal_json_decode($content);

    if (isset($decoded['encrypted'])) {

      // We have access to the site, return TRUE.
      return TRUE;
    } else {

      // We can't find what we're looking for.
      return FALSE;
    }
  } else {

    // We can't find what we're looking for.
    return FALSE;
  }
}

/**
 * This function may be misleading due to it's name.
 * This function will actually organise the update data into an array
 * that can be easily printed out on to the page.
 *
 * @param string $update_data The json encoded update data
 */
function samurai_render_update_data($update_data) {

  // Create the return array
  $return = array();
  $array_key = 0;

  // Decoode the json update_data
  $decoded = drupal_json_decode($update_data);

  // Split the core version number - we need the major number
  if (isset($decoded['drupal'])) {

    $core_version = samurai_split_version_number($decoded['drupal']['version']);
  } else {

    return NULL;
  }

  // Loop through each project.
  foreach ($decoded as $key => $value) {

    // Get the major project version
    $split_version_project = samurai_split_version_number($value['version']);

    // Get the stored project data.
    $result = db_query("SELECT
      name,
      major_version,
      latest_minor_version,
      latest_secure_minor_version,
      project_type,
      supported
      FROM {securitysamurai_projects}
      WHERE machine_name = :key
      AND core = :core
      AND major_version = :major_version
      AND project_type != 'module_custom'",
      array(
        ':key' => $key,
        ':major_version' => $split_version_project['major'],
        ':core' => $core_version['major'] . '.x'
    ))->fetchAll();

    if (!empty($result)) {

      $return[$array_key]['name'] = $result[0]->name;
      $return[$array_key]['type'] = str_replace('_', ' ', $result[0]->project_type);

      if ($value['name'] == 'Drupal core') {

        $return[$array_key]['installed_version'] = $value['version'];
      } else {

        $return[$array_key]['installed_version'] = substr($value['version'], 4);
      }

      $return[$array_key]['latest_version'] = $result[0]->major_version . '.' . $result[0]->latest_minor_version;
      $return[$array_key]['secure_version'] = $result[0]->major_version . '.' . $result[0]->latest_secure_minor_version;

      // If the module is unsupported.
      if ($result[0]->supported == 0) {
        $return[$array_key]['status'] = 'unsupported';
      }

      // Check if the project minor is equal to x e.g. 1.x 2.x
      // It is generally not recommended to install modules like this.
      if ($split_version_project['minor'] == 'x') {
        $return[$array_key]['status'] = 'unsecure';
      }

      // Check if the project minor is lest than the latest minor.
      if ($split_version_project['minor'] < $result[0]->latest_minor_version) {
        $return[$array_key]['status'] = 'secure';
      }

      // Check if the project minor is equal to the latest minor.
      if ($split_version_project['minor'] == $result[0]->latest_minor_version) {
        $return[$array_key]['status'] = 'latest';
      }

      // Check if the project minor is less than the latest secure minor.
      if ($split_version_project['minor'] < $result[0]->latest_secure_minor_version) {
        $return[$array_key]['status'] = 'unsecure';
      }

      $array_key++;
    }
  }
  if (!empty($return)) {

    return $return;
  } else {

    return NULL;
  }
}

/**
 * Returns a count of available security updates for a site
 *
 * @param array[mixed] $site_update_data The update data
 */
function samurai_count_available_security_updates($update_data) {

  $update_counter = 0;

  foreach ($update_data as $machine_name => $data) {

    if ($data['machine_name'] == 'drupal') {
      $version = samurai_split_version_number_core($data['version']);
      $secure_minor = samurai_project_secure_minor($data['machine_name'], $version['major'] . '.x', $data['version']);
    } else {
      $version = samurai_split_version_number($data['version']);
      $secure_minor = samurai_project_secure_minor($data['machine_name'], $data['core'], $data['version']);
    }
    if ($version['minor'] < $secure_minor) {

      $update_counter++;
    } else {

      continue;
    }
  }

  return $update_counter;
}

/**
 * Retrieve the core version of a particular site.
 *
 * @param string $site_name The name of the site.
 * @param string $site_update_data The json encoded site update data.
 */
function samurai_get_site_core_major($site_name, $site_update_data = NULL) {

  $update_data = '';

  // Get the site update data if left empty.
  if (is_null($site_update_data)) {
    $site_update_data = db_query('SELECT site_update_data FROM {securitysamurai_site}
      WHERE name = :name', array(
        ':name' => $site_name,
      ))->fetchAll();

    $update_data = '';
    foreach ($site_update_data as $key => $value) {
      $update_data = drupal_json_decode($value->site_update_data);
      if (isset($update_data['drupal']['version'])) {
        break;
      }
    }
  } else {
    $update_data = drupal_json_decode($site_update_data);
  }

  $split_version = samurai_split_version_number($update_data['drupal']['version']);

  return $split_version['major'] . '.x';
}

/**
 * Returns the alias
 */
function samurai_get_site_alias_id($site_id) {

}

/**
 * Returns a site id from the database.
 *
 * @param string $site_name The site name as present in the database.
 */
function samurai_get_site_id($site_name) {

  // Query the database and return the result.
  $result = db_query('SELECT id FROM {securitysamurai_site} WHERE
    name = :name', array(
      ':name' => $site_name
    ))->fetchField();

  if (!empty($result)) {
    return $result;
  } else {
    return NULL;
  }
}

/**
 * Returns the site alias
 *
 * @param int $site_id The unique ID of the client_site entity
 */
function samurai_get_site_alias($site_id) {

  // Query the database and return the result
  $result = db_query('SELECT site_alias_name FROM {securitysamurai_site} WHERE
    id = :id', array(
      ':id' => $site_id
    ))->fetchField();

  if (!empty($result)) {
    return $result;
  } else {
    return NULL;
  }
}

/**
 * Returns the site update checking method
 *
 * @param int $site_id The unique ID of the client_site entity
 */
function samurai_get_site_update_method($site_id) {

  // Query the database and return the result
  $result = db_query('SELECT update_check_method FROM {securitysamurai_site} WHERE
    id = :id', array(
      ':id' => $site_id
    ))->fetchField();

  if (!empty($result)) {
    return $result;
  } else {
    return NULL;
  }
}

/**
 * Returns a list of sites in an array.
 */
function samurai_get_sites() {

  // Query the database.
  $result = db_query('SELECT name FROM {securitysamurai_site}')->fetchAll();

  if (!empty($result)) {

    // Return the result as an array, and not
    // the object it was received as.
    $return = array();
    foreach ($result as $name) {
      $return[] = $name->name;
    }
    return $return;
  } else {
    return NULL;
  }
}
