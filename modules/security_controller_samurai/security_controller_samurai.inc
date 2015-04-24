<?php
/**
 * @file
 * General helper functions that have no specific place.
 *
 * @author jkswoods
 */

/**
 * Retrieve an array of clients affected by a particular security announcement.
 *
 * @param string $sa_id The security announcement ID.
 * @param string $project_name The name of the affected project.
 * @param string $core The json encoded string of compatible core version/s.
 *
 * @return string $return HTML content to return.
 * @return NULL Returned when there is no result from the query.
 *
 * @todo Will also need to make sure the isntalled version minor is less than the security update minor.
 */
function samurai_get_affected_clients($sa_id, $project_name, $core) {
  // Now retrieve the data on the client if they have the project in the update_data
  if (samurai_sa_exists($sa_id)) {
    $results = db_query('SELECT * FROM {securitysamurai_client} WHERE
      INSTR(site_update_data, :project_name)',
      array(
        ':project_name' => $project_name
      ))->fetchAll();
    // Decode the JSON into a PHP array.
    $core_array = drupal_json_decode($core);
    // Check that a result was returned.
    if (!empty($results)) {
      // Check if is a compatible core version.
      $return = '';
      foreach ($results as $result => $r) {
        // Check that the core versions are matching.
        $site_update = drupal_json_decode($r->site_update_data);
        // Split the version number of Drupal core into major and minor.
        $split_version_core = samurai_split_version_number($site_update['drupal']['version']);
        // Retrieve the machine name of the affected project
        $machine_name = samurai_project_machine_name($project_name);
        // Split the version number of the project into major and minor.
        $split_version_contrib = samurai_split_version_number($site_update[$machine_name]['version']);
        // Loop through each affected core version.
        foreach ($core_array as $core) {
          // Compare the core versions.
          if ($core == $split_version_core['major'] . '.x') {
            // Check that the clients minor version is less than the secure version.
            if ($split_version_contrib['minor'] < samurai_project_secure_minor($machine_name, $core, $split_version_contrib['major'])) {
              // If the core versions are correct, return the client markup.
              $return .= '<p class="content"><a href="/client/' . $r->id . '">' . $r->name . '</a></p>';
            }
          }
        }
      }
      // Return the result.
      return $return;
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
  // Get the starting position of the risk classification.
  $risk_pos_start = strpos($risk_raw, '(') + 1;
  // Get the finishing position of the risk classification.
  $risk_pos_end = strpos($risk_raw, ')');
  // Calculate the length of the risk classification.
  $risk_length = $risk_pos_end - $risk_pos_start;
  // Retrieve the risk classification from the  
  $risk = substr($risk_raw, $risk_pos_start, $risk_length);
  // Remove spaces from the result.
  $return = strtolower(str_replace(' ', '', $risk));
  // Return the result.
  return $return;
}

/**
 * Check if a client exists in the database.
 *
 * @param string $client_name The name of the client.
 * @param string $client_url The url of the client's site.
 *
 * @return bool TRUE/FALSE The client exists in the database?
 */
function samurai_client_exists($client_name, $client_url) {
  // Query the database. Faster than using entity load.
  $result = db_query('SELECT id FROM {securitysamurai_client} WHERE
    name = :name AND
    site_url = :site_url',
    array(
      ':name' => $client_name,
      ':site_url' => $client_url,
    ))->fetchField();

  if (!empty($result)) {
    // The result isn't empty.
    // Client exists. Return TRUE.
    return TRUE;
  } else {
    // The result is empty.
    // Client doesn't exist. Return FALSE.
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
    // Query the database. Faster than using entity load.
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
      ':major_version' => $major_version,
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
 * @param string $minor_version The minor version of the project e.g. 1.x
 * @param int $supported 1 OR 0, 1 for supported, 0 for unsupported.
 *
 * @todo use the entity way of creating projects.
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
    // The project has not yet been added to the database.
    // We must add it to the database.
    db_insert('securitysamurai_projects')
    ->fields(array(
      'name' => $name,
      'machine_name' => $machine_name,
      'project_type' => $type,
      'major_version' => $major_version,
      'latest_minor_version' => (int)$latest_minor_version,
      'latest_secure_minor_version' => (int)$latest_secure_minor_version,
      'core' => $core,
      'supported' => $supported,
    ))
    // Execute the query.
    ->execute();
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
  // If str2 isn't null then we want the between str1 and str2.
  if (is_null($str2)) {
    // If $str2 is null then it can't be used.
    $string1_start = strpos($xml_string, $str1);
    // Work out the starting position of the string to return.
    $return_start = $string1_start + (strlen($str1) + 1);
    // Retrieve the returning value.
    $return = substr($xml_string, $return_start);
  } else {
    // Get the start position of the two specified strings.
    // How do you get the position of something that is at the end of the $xml_string string?
    $string1_start = strpos($xml_string, $str1);
    $string2_start = strpos($xml_string, $str2);
    // Work out the starting position of the string to return.
    $return_start = $string1_start + (strlen($str1) + 1);
    // Work out the length of the string to return.
    $return_length = $string2_start - $return_start - 1;
    // Retrieve the returning value.
    $return = substr($xml_string, $return_start, $return_length);
  }
  // Return $return.
  return $return;
}

/**
 * Decrypt the update data retrieved from a client site.
 *
 * @param string $update_data The encrypted update data string.
 * @param string $encryption_key The encryption key.
 *
 * @return string $decrypted The decrypted update data.
 */
function samurai_decrypt_update_data($update_data, $encryption_key) {
  // Create the SHA key from the encryption key.
  $key = hash('SHA256', $encryption_key, TRUE);
  // Base 64 decode the update data.
  $ciphertext_dec = base64_decode($update_data);
  // Get the IV from $ciphertext_dec.
  $iv_size = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_CBC);
  $iv_dec = substr($ciphertext_dec, 0, $iv_size);
  // Remove the IV from $ciphertext_dec.
  $ciphertext_dec = substr($ciphertext_dec, $iv_size);
  // Decrypt $ciphertext_dec.
  $plaintext_dec = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, $ciphertext_dec, MCRYPT_MODE_CBC, $iv_dec);
  // Sometimes there can be problems with \u0022 appearing.
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
 *
 * @todo
 */
function _samurai_mark_project_type($project_name, $type) {
  // Not implemented
  // Maybe not needed thanks to _samurai_get_project_type?
}

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
  // Get the position of the splitter.
  $splitter = strpos($version_number, '.');
  $minor = substr($version_number, $splitter + 1);
  $major = substr($version_number, 0, $splitter);
  // Remove the spaces from the split version number
  $minor = str_replace(' ', '', $minor);
  $major = str_replace(' ', '', $major);
  // Add the values to the return array.
  $return['minor'] = $minor;
  $return['major'] = $major;
  return $return;
}

/**
 * Retrieve the latest secure version as stored in the database.
 *
 * @param string $machine_name The machine name of the project.
 * @param string $core The compatible core version e.g. 7.x, 6.x
 * @param string $major_version The major version of the project.
 *
 * @todo FINISH HIM!
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
