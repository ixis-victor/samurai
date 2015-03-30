<?php
/**
 * @file
 * functions to retrieve available update data for the modules and themes
 */

// Retrieve the latest update data from Drupal.org with
// http://updates.drupal.org/release-history
// 
// Example use:
// http://updates.drupal.org/release-history/views/7.x
// Will return XML of the views project.


/**
 * @todo Add a check to see if a module is unsupported.
 */

define('SAMURAI_ORG_UPDATE_URL', 'http://updates.drupal.org/release-history/');

function security_controller_samurai_cron() {
  drupal_set_message('Cron ran ' . REQUEST_TIME);
}

function _samurai_curl_request($url) {
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
function _samurai_get_update_data($site_url) {
  // Create a new curl object
  $content = _samurai_curl_request($site_url . '/admin/reports/security_samurai');
  // Return the result.
  return $content;
}

/**
 * Add the update data to the securitysamuraiclient_projects table.
 *
 * @param array[mixed] $decrypted_update_data Decrypted update data array.
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
      $name = $m['name'];
      // Machine name
      $machine_name = $m['machine_name'];
      // Compatible core version
      $core = $m['core'];
      // The type of module e.g. module_custom, module_contrib
      $type = 'module' . _samurai_get_project_type($machine_name, $core);
      // Split the version number into the minor and major versions.
      $split_version = _samurai_split_version_number($m['version']);
      // Add the project.
      _samurai_add_project($name, $machine_name, $type, $core, $split_version['major'], $split_version['minor']);
      // Add the module on to the projects_array
      $projects_array[$m['machine_name']] = $m;
    }
  }
  // Process theme projects
  if (!empty($themes)) {
    foreach ($themes as $theme => $t) {
      // Human readable name
      $name = $t['name'];
      // Machine name
      $machine_name = $t['machine_name'];
      // Compatible core version
      $core = $t['core'];
      // The type of theme e.g. theme_custom, theme_contrib.
      $type = 'theme' . _samurai_get_project_type($machine_name, $core);
      // Split the version number into the minor and major versions.
      $split_version = _samurai_split_version_number($t['version']);
      // Add the project.
      _samurai_add_project($name, $machine_name, $type, $core, $split_version['major'], $split_version['minor']);
      // Add the theme on to the projects_array
      $projects_array[$t['machine_name']] = $t;
    }
  }
  // Process core
  if (!empty($core)) {
    /**
     * @todo
     */
  }
  return $projects_array;
}

/**
 * Gets a project type e.g. Contrib or Custom
 */
function _samurai_get_project_type($machine_name, $core) {
  // Create the new curl request.
  $content = _samurai_curl_request(SAMURAI_ORG_UPDATE_URL . $machine_name . '/' . $core);
  // Check if the project exists
  $not_exist = strpos($content, '<error>No release history');
  // Check the value and return a relevant value
  if ($not_exist) {
    return '_custom';
  } else {
    return '_contrib';
  }
}

/**
 * Adds a project to the securitysamuraiclient_projects table.
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
 * @param string $major_version The major version of the project x.1
 * @param string $minor_version The minor version of the project 1.x
 */
function _samurai_add_project($name, $machine_name, $type, $core, $major_version, $minor_version) {
  // Check first that the project isn't already added.
  /**
   * @todo If the project needs to be checked anywhere else, create a function for it.
   */
  $result = db_query('SELECT name FROM {securitysamuraiclient_projects} WHERE 
    name = :name AND 
    core = :core AND 
    major_version = :majversion', 
    array(
      ':name' => $name,
      ':core' => $core,
      ':majversion' => $major_version,
    )
  )->fetchField();
  if (empty($result)) {
    // The project has not yet been added to the database.
    // We must add it to the database.
    db_insert('securitysamuraiclient_projects')
    ->fields(array(
      'name' => $name,
      'machine_name' => $machine_name,
      'project_type' => $type,
      'major_version' => $major_version,
      'minor_version' => $minor_version,
      'core' => $core,
    ))
    ->execute();
  }
  /**
   * @todo if it is a contrib module check for the latest secure version.
   */
}

/**
 * Checks whether a project is already in the database.
 *
 * @param string $machine_name The machine name of the project.
 *
 * @todo
 */
function _samurai_project_exists($machine_name) {
  // Not implemented
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
}

/**
 * Splits a version number into their major and minor numbers.
 *
 * @param string $version_number The unsplit version number e.g. 1.14.
 *
 * @return array[mixed] $split The split version number.
 */
function _samurai_split_version_number($version_number) {
  $return = array();
  if (strpos($version_number, '7.x-') !== FALSE) {
    // Cater for version numbers that contain 7.x-
    $version_number = substr($version_number, 4);
  }
  // Get the position of the splitter.
  $splitter = strpos($version_number, '.');
  $minor = substr($version_number, $splitter + 1);
  $major = substr($version_number, 0, $splitter);
  // Add the values to the return array.
  $return['minor'] = $minor;
  $return['major'] = $major;
  return $return;
}

/**
 * Decrypt the update data retrieved from a client site.
 *
 * @param string $update_data The encrypted update data string.
 * @param string $encryption_key The encryption key.
 */
function _samurai_decrypt_update_data($update_data, $encryption_key) {
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