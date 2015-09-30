<?php
/**
 * @file
 * General helper functions that have no specific place.
 *
 * @author jkswoods
 */

/**
 * Functions for manipulating and retrieving
 * client_site entity data
 */
class SecuritySamuraiControllerClientSite {

  // Client site entity variables
  public $id;
  public $name;
  public $update_data;
  public $encryption_key;
  public $site_url;
  public $created_at;
  public $updated_at;
  public $update_count;
  public $update_next_check;
  public $update_check_method;
  public $core_version;
  public $site_alias_name;
  public $site_alias_id;
  public $auto_updates;

  /**
   * Class constructor
   *
   * @param int $id The ID of the client_site entity
   */
  public function __construct($id = NULL) {
    // If the ID parameter is set, load the client_site
    // entity and assign variables
    if (!is_null($id) && $this->exists($id)) {
      $client_site = client_site_load($id);
      $this->id = $client_site->id;
      $this->name = $client_site->name;
      $this->update_data = drupal_json_decode($client_site->site_update_data);
      $this->site_url = $client_site->site_url;
      $this->created_at = $client_site->created_at;
      $this->updated_at = $client_site->updated_at;
      $this->update_count = $client_site->update_count;
      $this->update_next_check = $client_site->update_next_check;
      $this->update_check_method = $client_site->update_check_method;
      $this->core_version = $client_site->core_version;
      $this->site_alias_name = $client_site->site_alias_name;
      $this->site_alias_id = $client_site->site_alias_id;
      $this->auto_updates = $client_site->auto_updates;
    }
  }

  /**
   * Check if an entity of client_site exists
   *
   * @param int $id The ID of the client_site entity
   *
   * @return bool TRUE/FALSE
   */
  public function exists($id) {
    // Query the database for a row with the correct ID
    $result = db_query('SELECT id FROM {securitysamurai_site}
      WHERE id = :id', array(
        ':id' => $id
      ))->fetchField();
    // If the result isn't empty, return TRUE
    // else, return FALSE
    if (!empty($result)) {
      return TRUE;
    } else {
      return FALSE;
    }
  }

  /**
   * Check if an entity of client_site exists by site_name
   *
   * @param string $name The name of the client_site entity
   *
   * @return bool TRUE/FALSE
   */
  public function exists_by_name($name) {
    // Query the database for a row with the correct name
    $result = db_query('SELECT id FROM {securitysamurai_site}
      WHERE name = :name', array(
        ':name' => $name
      ))->fetchField();
    // If the result isn't empty, return TRUE
    // else, return FALSE
    if (!empty($result)) {
      return TRUE;
    } else {
      return FALSE;
    }
  }

  /**
   * Returns an array of all the sites in the
   *
   * @return array[int] $result_return An array of site ID values
   */
  public function retrieve_sites_all_ids() {
    // Query the database for all site ID values
    $result = db_query('SELECT id FROM {securitysamurai_site}')
      ->fetchAll();
    // Reconstruct the result into a tidier array
    $result_return = array();
    foreach ($result as $key => $value) {
      $result_return[] = $value->id;
    }
    // Return the result
    return $result_return;
  }

  /**
   * Return an array of sites with the specified project in the
   * site_update_data
   *
   * @param string $project_name The name of the project
   *
   * @return array[mixed] The array of site_ids that have the project
   */
  public function retrieve_sites_with_project($project_name) {
    // Retrieve all site IDs
    $site_ids = $this->retrieve_sites_all_ids();
    // Loop through each site ID, checking the site_update_data
    $sites_with_project = array();
    foreach ($site_ids as $site_id => $value) {
      $client_site = client_site_load($value);
      $site_update_data = drupal_json_decode($client_site->site_update_data);
      if (isset($site_update_data[$project_name])) {
        if ($project_name == 'drupal') {
          $helper = new SecuritySamuraiControllerHelper;
          $helper->split_version_number($site_update_data[$project_name]['version']);
          $sites_with_project[] = array(
            'site_id' => $value,
            'version' => $site_update_data[$project_name]['version'],
            'core' => $helper->version_major . '.x',
          );
        } else {
          $sites_with_project[] = array(
            'site_id' => $value,
            'version' => $site_update_data[$project_name]['version'],
            'core' => $site_update_data[$project_name]['core'],
          );
        }
      }
    }
    return $sites_with_project;
  }

  /**
   * Returns a count of available security updates for a
   * particular site
   *
   * @param array[mixed] $site_update_data The site update data
   *
   * @return bool FALSE If no value for $update_data is set
   * @return int $update_counter The security update count
   */
  public function count_available_security_updates($update_data = NULL) {
    // Set a value for $update_data if it doesn't have one already
    if (is_null($update_data) && isset($this->update_data)) {
      $update_data = $this->update_data;
    } else {
      return FALSE;
    }
    // Initialise variables
    $projects = new SecuritySamuraiControllerProject();
    $helper = new SecuritySamuraiControllerHelper;
    $helper->split_version_number($update_data['drupal']['version']);
    $drupal_major = $helper->version_major . '.x';
    $update_counter = 0;
    // Loop through each update data entry, checking if the project is at
    // the latest secure version
    foreach ($update_data as $machine_name => $data) {
      // Split the version of the project
      $helper->split_version_number($data['version']);
      $secure_minor = $projects->retrieve_latest_secure_minor($data['machine_name'], $drupal_major, $version['major']);
      // Check if the version minor is less than the secure_minor
      if ($helper->version_minor < $secure_minor) {
        $update_counter++;
      } else {
        continue;
      }
    }
    // Return the update_counter
    $this->update_count = $update_counter;
    return $update_counter;
  }

  /**
   * Returns the site update data of a client_site
   *
   * @param string $site_id The ID of the client_site entity
   *
   * @return array[mixed] The site update data
   * @return bool FALSE If no value for $update_data is set
   */
  public function retrieve_update_data($site_id) {
    // Query the database
    $update_data = db_query('SELECT site_update_data FROM {securitysamurai_site}
      WHERE id = :id', array(
        ':id' => $site_id,
      ))->fetchField();
    // If $update_data isn't empty, return $update_data
    // else, return FALSE
    if (!empty($update_data)) {
      $update_data = drupal_json_decode($update_data);
      $this->update_data = $update_data;
      return $update_data;
    } else {
      return FALSE;
    }
  }

  /**
   * Retrieve an array of projects which require security updates
   * for a project
   * Filters out projects that are not set to have
   *
   * @param int $site_id The entity ID of the client_site
   *
   * @return bool FALSE When no value for site_id or update_data is set
   * @return array[mixed] The projects that require a security update
   */
  public function retrieve_security_updates($site_id = NULL) {
    // Retrieve the site_update_data
    $update_data = array();
    if (is_null($site_id) && isset($this->update_data)) {
      $update_data = $this->update_data;
    } elseif (isset($site_id)) {
      $update_data = $this->retrieve_update_data($site_id);
    } else {
      return FALSE;
    }
    // Initialise classes that will be used
    $helper = new SecuritySamuraiControllerHelper;
    $projects = new SecuritySamuraiControllerProject();
    // Initialise an array for the projects which will require a
    // security update
    $security_updates = array();
    // Loop through each project in the update data array
    foreach ($update_data as $machine_name => $data) {
      // The Drupal version entry is structured a little differently
      // compared to normal modules and themes
      if ($data['auto_updates'] == 1) {
        if ($machine_name == 'drupal') {
          // Check for the latest_secure_minor_version of Drupal
          $helper->split_version_number($data['version']);
          $core_version = $helper->version_major . '.x';
          $secure_minor = $projects->retrieve_latest_secure_minor($data['machine_name'],
            $core_version, $helper->version_major);
          if ($helper->version_minor < $secure_minor) {
            // If project is not secure, add it to the secure
            // update array
            $security_updates[$data['machine_name']] = array(
              'new_version' => $helper->version_major . '.' . $secure_minor,
            );
          }
        } else {
          // Retrieve the latest secure minor for the current project
          $helper->split_version_number($data['version']);
          $secure_minor = $projects->retrieve_latest_secure_minor($data['machine_name'],
            $data['core'], $helper->version_major);
          if ($helper->version_minor < $secure_minor) {
            // If the project isn't the latest secure version
            // add the project to the secure update array with the latest
            // secure version to update to
            $security_updates[$project['machine_name']] = array(
              'new_version' => $helper->version_major . '.' . $secure_minor,
            );
          }
        }
      }
    }
    // Return the array of projects
    return $security_updates;
  }
}

/**
 * Functions for manipulating and retrieving
 * security_announcement entity data
 */
class SecuritySamuraiControllerSA {

  // Security announcement variables
  public $id;
  public $sa_id;
  public $risk_level;
  public $date;
  public $version;
  public $vulnerability;
  public $solution;
  public $project_name;
  public $secure_version;

  /**
   * Class contructor
   *
   * @param int $id The ID of the client_site entity
   */
  public function __construct($id = NULL) {
    // If ID parameter is set, check if the security_announcement
    // entity exists
    if (!is_null($id) && $this->exists($id)) {
      // Load the entity and assign the variables
      $sa_entity = security_announcement_load($id);
      $this->id = $sa_entity->id;
      $this->sa_id = $sa_entity->sa_id;
      $this->risk_level = $sa_entity->risk_level;
      $this->date = $sa_entity->date;
      $this->version = drupal_json_decode($sa_entity->version);
      $this->vulnerability = $sa_entity->vulnerability;
      $this->solution = $sa_entity->solution;
      $this->project_name = $sa_entity->project_name;
      $this->secure_version = $sa_entity->secure_version;
    }
  }

  /**
   * Check if an entity of security_announcement exists
   *
   * @param int $id The ID of the security_announcement entity
   *
   * @return bool TRUE/FALSE
   */
  public function exists($id) {
    // Query the database for a row with the correct ID
    $result = db_query('SELECT id FROM securitysamurai_sa
      WHERE id = :id', array(
        ':id' => $id
      ))->fetchField();
    // If the result isn't empty, return TRUE
    // else, return FALSE
    if (!empty($result)) {
      return TRUE;
    } else {
      return FALSE;
    }
  }

  /**
   * Check if an entity of security_announcement exists by
   * checking with the sa ID
   *
   * @param string $sa_id The security announcement ID issued by drupal.org
   *
   * @return bool TRUE/FALSE
   */
  public function exists_by_sa($sa_id) {
    // Query the database for a row with the correct ID
    $result = db_query('SELECT id FROM securitysamurai_sa
      WHERE sa_id = :sa_id', array(
        'sa_id' => $sa_id
      ))->fetchField();
    // If the result isn't empty, return TRUE
    // else, return FALSE
    if (!empty($result)) {
      return TRUE;
    } else {
      return FALSE;
    }
  }

  /**
   * Returns an array of sites affected by a security announcement
   *
   * @param string $project_name The name of the project to check
   * @param string $core The JSON encoded string of compatible core versions
   * @param string $sa_id The security announcement ID
   *  - This variable will default to the sa_id set in the variable $this->sa_id
   *    if no value is set
   *
   * @return array[int] An array of site IDs affected by a security announcement
   * @return FALSE Returned when there is no result from the query
   */
  public function client_sites_affected($project_name, $core, $sa_id = NULL) {
    // Set a value for $sa_id, if it doesn't have one already
    if (is_null($sa_id) && !is_null($this->sa_id)) {
      $sa_id = $this->sa_id;
    }
    // Strip whitespace project_name
    $project_name = trim($project_name);
    // Perform a check to see if the security_announcement entity exists
    // before continuing
    if ($this->exists_by_sa($sa_id)) {
      // Retrieve the machine name of the project
      $project = new SecuritySamuraiControllerProject();
      $project->retrieve_project_machine_name($project_name);
      // Check the sites which contain the specified project
      $client_site = new SecuritySamuraiControllerClientSite();
      $sites = $client_site->retrieve_sites_with_project($project->machine_name);
      // Compare the version on each site to the latest secure version
      // in the database
      $affected_sites_array = array();
      foreach ($sites as $site => $value) {
        $helper = new SecuritySamuraiControllerHelper;
        $helper->split_version_number($value['version']);
        $project->retrieve_latest_secure_minor($project->machine_name, $value['core'], $helper->version_major);
        if ($project->latest_secure_minor_version > $helper->version_minor) {
          $affected_sites_array[] = $value['site_id'];
        }
      }
      return $affected_sites_array;
    }
  }

  /**
   * Returns a HTML list of links to sites affected by a security announcement
   *
   * @param string $project_name The name of the project to check
   * @param string $core The JSON encoded strign of compatible core versions
   * @param string $sa_id THe security announcement ID
   *  - This variable will default to the sa_id set in the variable $this->sa_id
   *    if no value is set
   *
   * @return string $return_html HTML of affected sites
   * @return NULL Returned if a value for $return_html is not set
   */
  public function client_sites_affected_html($project_name, $core, $sa_id = NULL) {
    // Set a value for $sa_id, if it doesn't have one already
    if (is_null($sa_id) && !is_null($this->sa_id)) {
      $sa_id = $this->sa_id;
    }
    // Call the client_sites_affected function
    $affected_sites_array = $this->client_sites_affected($project_name, $core, $sa_id);
    // Loop through each site, adding it to the HTML
    $return_html = '';
    foreach ($affected_sites_array as $site => $value) {
      $client_site = new SecuritySamuraiControllerClientSite($value);
      $return_html .= '<p class="content"><a href="/client-site/' . $value . '">' . $client_site->name . '</a></p>';
    }
    // Return the HTML
    if (strlen($return_html) > 0) {
      return $return_html;
    } else {
      return NULL;
    }
  }
}

/**
 * Functions for manipulating and retrieving
 * project entity data
 */
class SecuritySamuraiControllerProject {

  // Project entity variables
  public $id;
  public $name;
  public $machine_name;
  public $major_version;
  public $latest_minor_version;
  public $latest_secure_minor_version;
  public $core;
  public $supported;
  public $project_type;
  public $update_next_check;

  /**
   * The class constructor
   */
  public function __construct($id = NULL) {
    // If ID parameter is set, check if the security_announcement
    // entity exists
    if (!is_null($id) && $this->exists($id)) {
      // Load the entity and assign the variables
      $project = project_load($id);
      $this->id = $project->id;
      $this->name = $project->name;
      $this->machine_name = $project->machine_name;
      $this->major_version = $project->major_version;
      $this->latest_minor_version = $project->latest_minor_version;
      $this->latest_secure_minor_version = $project->latest_secure_minor_version;
      $this->core = $project->core;
      $this->supported = $project->supported;
      $this->project_type = $project->project_type;
      $this->update_next_check = $project->update_next_check;
    }
  }

  /**
   * Check if an entity of project exists
   *
   * @param int $id The ID of the project entity
   *
   * @return bool TRUE/FALSE
   */
  public function exists($id) {
    // Query the database for a row with the correct ID
    $result = db_query('SELECT id FROM securitysamurai_projects
      WHERE id = :id', array(
        ':id' => $id
      ))->fetchField();
    // If the result isn't empty, return TRUE
    // else, return FALSE
    if (!empty($result)) {
      return TRUE;
    } else {
      return FALSE;
    }
  }

  /**
   * Check if an entity of project exists by:
   * - Machine name
   * - Core version
   * - Major version number
   *
   * @param string $machine_name The machine name of the project
   * @param string $core_version The core version string of the project
   * @param int $major_version The major version of the project
   *
   * @return bool TRUE/FALSE
   */
  public function exists_by_data($machine_name, $core_version, $major_version) {
    // Query the database for the project
    $result = db_query('SELECT id FROM {securitysamurai_projects} WHERE
      machine_name = :machine_name AND
      core = :core AND
      major_version = :major_version',
      array(
        ':machine_name' => $machine_name,
        ':core' => $core,
        ':major_version' => $major_version
      )
    )->fetchField();
    // If the result is not empty, the project exists - return TRUE
    // else, return FALSE
    if (!empty($result)) {
      return TRUE;
    } else {
      return FALSE;
    }
  }

  /**
   * Create a new projects entity
   * Adds an project entry to the securitysamurai_projects table
   *
   * @param string $name The human-readable name of the project
   * @param string $machine_name The machine-readable name of the project
   * @param string $type The project type
   * @param int $major_version The major version of the project
   * @param int $latest_minor_version The minor version of the project
   * @param int $latest_secure_minor_version The latest secure minor version
   * of the project
   * @param int $supported Is the project supported? 1 OR 0
   */
  public function add($name, $machine_name, $type, $core, $major_version,
    $latest_minor_version, $latest_secure_minor_version, $supported = NULL) {
    // Check if the project exists
    if ($this->exists_by_data($machine_name, $core, $major_version)) {
      // The project has not yet been added
      // Create a new project entity for this project
      $project = entity_create('project', array(
        'name' => $name,
        'machine_name' => $machine_name,
        'project_type' => $type,
        'major_version' => (int)$major_version,
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
   * Retrieve a project machine name
   *
   * @param string $project_name Human-readable project name
   *
   * @return string $machine_name The machine name for the project
   */
  public function retrieve_project_machine_name($project_name) {
    // Query the database for a machine name of the project
    $machine_name = db_query('SELECT machine_name FROM {securitysamurai_projects}
      WHERE name = :name', array(
        ':name' => $project_name
      ))->fetchField();
    // If the result isn't empty, return it
    // else, return FALSE
    if (!empty($machine_name)) {
      $this->machine_name = $machine_name;
      return $machine_name;
    } else {
      return FALSE;
    }
  }

  /**
   * Retrieve the latest secure minor for a specific project
   *
   * @param string $machine_name The machine name of the project
   * @param string $core The compatible core version of the project
   * @param int $major_version The major version number of the project
   *
   * @return int $secure_minor The latest secure minor version
   * @return bool FALSE Returned if a value for $secure_minor isn't set
   */
  function retrieve_latest_secure_minor($machine_name, $core, $major_version) {
    // Query the database for the latest secure version of the
    // project
    $latest_secure_minor = db_query('SELECT latest_secure_minor_version FROM {securitysamurai_projects}
      WHERE machine_name = :mn AND
      core = :c AND
      major_version = :mv', array(
        ':mn' => $machine_name,
        ':c' => $core,
        ':mv' => $major_version
      ))->fetchField();
    // If the result not empty, return it
    // else, return FALSE
    if (!empty($latest_secure_version)) {
      $this->latest_secure_minor_version = $latest_secure_minor;
      return $latest_secure_minor;
    } else {
      return FALSE;
    }
  }
}

/**
 * Functions that have no direct link to any other entities
 * however, are useful none the less
 */
class SecuritySamuraiControllerHelper {

  // Helper class variables
  public $version_major;
  public $version_minor;

  /**
   * Retrieve the version major and version minor
   * from a version string
   *
   * @param string $version The version number
   *
   * @return int $version_major The major version number
   */
  public function split_version_number($version) {
    // Remove the '7.x-' from version numbers
    if (strpos($version, '7.x-') !== FALSE) {
      $version = substr($version, 4);
    }
    // Remove the version extension e.g. -beta1
    if (strpos($version, '-') !== FALSE) {
      $pos = strpos($version, '-');
      $version = substr($version, 0, $pos);
    }
    // Remove the version extension e.g. +rc1
    if (strpos($version, '+') !== FALSE) {
      $pos = strpos($version, '+');
      $version = substr($version, 0, $pos);
    }
    // Get the position of the splitter.
    $splitter = strpos($version, '.');
    $minor = substr($version, $splitter + 1);
    $major = substr($version, 0, $splitter);
    // Remove the spaces from the split version number
    $minor = str_replace(' ', '', $minor);
    $major = str_replace(' ', '', $major);
    // Set the results to the public variables
    $this->version_minor = $minor;
    $this->version_major = $major;
  }

  /**
   * Retrieve the risk category e.g. Low, moderately critical etc.
   *
   * @param string $risk The risk string
   *
   * @return string $return The risk level
   */
  public function parse_risk($risk) {
    // - Get the starting position of the risk classification.
    // - Get the finishing position of the risk classification.
    // - Calculate the length of the risk classification.
    // - Retrieve the risk classification.
    // - Remove spaces from the result.
    $risk_pos_start = strpos($risk, '(') + 1;
    $risk_pos_end = strpos($risk, ')');
    $risk_length = $risk_pos_end - $risk_pos_start;
    $risk = substr($risk, $risk_pos_start, $risk_length);
    $return = strtolower(str_replace(' ', '', $risk));

    // Return the result.
    return $return;
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
 *
 * NOT YET IMPLEMENTED
 */
function samurai_get_site_alias_id($site_id) {}

/**
 * Returns the site update count
 *
 * @param int $site_id The entity ID of the client_site entity
 */
function smaurai_get_site_update_count($site_id) {

  // Query the database
  $result = db_query('SELECT update_count FROM {securitysamurai_site} WHERE
    id = :site_id', array(
      ':site_id' => $site_id,
    ))->fetchField();

  if (!empty($result)) {
    return $result;
  } else {
    return NULL;
  }
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

/**
 * Returns the site update data from the client_site entity
 *
 * @param int $site_id The entity ID of the client_site
 */
function samurai_get_site_update_data_entity($site_id) {

  $result = db_query('SELECT site_update_data FROM {securitysamurai_site}
    WHERE id = :site_id', array(
      ':site_id' => $site_id
    ))->fetchField();

  if (!empty($result)) {
    return $result;
  } else {
    return NULL;
  }
}

/**
 * Returns TRUE/FALSE for auto-updates being enabled for
 * a particular project
 *
 * @param int $site_id The entity ID of the current client_site
 * @param string $machine_name The machine name of the project to check
 */
function samurai_project_auto_update_enabled($site_id, $machine_name) {

  // Retrieve the site_update_data
  $site_update_data = samurai_get_site_update_data_entity($site_id);
  // Decode the site_update_data
  $site_update_data = drupal_json_decode($site_update_data);
  // Check if the array is set
  if (isset($site_update_data[$machine_name]['auto_updates'])) {
    // Return true if auto-update is enabled and false if not
    if ($site_update_data[$machine_name]['auto_updates'] == 1) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

/**
 * Returns TRUE/FALSE for auto-updates being enabled for an entire
 * site
 *
 * @param int $site_id The entity ID of the current client_site
 */
function samurai_projects_auto_update_enabled($site_id) {

  // Query the database for the value of auto_updates
  $result = db_query('SELECT auto_updates FROM {securitysamurai_site}
    WHERE id = :site_id', array(
      ':site_id' => $site_id,
    ))->fetchField();
  // If the value of auto_updates is NULL, then it hasn't
  // been disabled
  if (is_null($result)) {
    return TRUE;
  } else {
    if ($result == 0) {
      return FALSE;
    } else {
      return TRUE;
    }
  }
}
