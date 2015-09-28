<?php
/**
 * @file
 * PHP API Wrapper for Deploy HQ
 */

class SamuraiDeployHQClient {

  // Public domain variable
  protected $wrapper;

  /**
   * Initialise a new SamuraiDeployHQWrapper
   */
  public function __construct($domain, $key, $user) {

    $this->wrapper = new SamuraiDeployHQWrapper($domain);
    $this->wrapper->setAuthentication($user, $key);
  }

  /**
   * Returns an array of projects including the project
   * information
   */
  public function getProjects() {
    return $this->wrapper->GET('projects');
  }

  /**
   * Returns an array of deployments for a specific
   * project including the deployment information
   *
   * @param string $project_permalink The human readable name of the project
   */
  public function getDeployments($project_permalink) {
    return $this->wrapper->GET('projects/' . $project_permalink . '/deployments');
  }

  /**
   * Returns the array of information for a specific
   * project
   *
   * @param string $project_permalink The machine readable project name
   */
  public function getProject($project_permalink) {
    return $this->wrapper->GET('projects/' . $project_permalink);
  }

  /**
   * Returns the array of information for a specific
   * deployment
   *
   * @param string $project_permalink The machine readable name of the project
   * @param string $deployment_uuid The unique ID for the deployment
   */
  public function getDeployment($project_permalink, $deployment_uuid) {
    return $this->wrapper->GET('projects/' . $project_permalink . '/deployments/' . $deployment_uuid);
  }

  /**
   * Creates a new project in DeployHQ
   *
   * @param string $project_name The human readable name of the project
   */
  public function createProject($project_name) {

    // Create the array of arguments
    // and json encode it
    $arguments = array(
      'project' => array(
        'name' => $project_name
      ),
    );
    $document = json_encode($arguments);

    // Perform the POST request
    return $this->wrapper->POST('projects', $document);
  }

  /**
   * Creates a new deployment in DeployHQ
   *
   * @param string $project_permalink THe machine readable name of the project
   */
  public function createDeployment($project_permalink) {
    return $this->wrapper->POST('projects/' . $project_permalink . '/deployments');
  }

  /**
   * Deletes a project in DeployHQ
   *
   * @param string $project_permalink The machine readable project name
   */
  public function deleteProject($project_permalink) {
    return $this->wrapper->DELETE('projects/' . $project_permalink);
  }
}

class SamuraiDeployHQWrapper {

  // Public domain variable
  public $domain;

  // Protected variables
  // - Can contain some sensitive data
  protected $curl;
  protected $key;
  protected $user;

  public function __construct($domain) {
    $this->domain = $domain;
  }

  public function setAuthentication($user, $key) {
    $this->key = $key;
    $this->user = $user;
  }

  public function GET($path) {
    return $this->execute('GET', $path);
  }

  public function POST($path, $document) {
    return $this->execute('POST', $path, $document);
  }

  public function PUT($path, $document) {
    return $this->execute('PUT', $path, $document);
  }

  public function DELETE($path) {
    return $this->execute('DELETE', $path);
  }

  protected function execute($method, $path, $data = null) {
    $this->curl = curl_init();

    curl_setopt_array($this->curl, array(
      CURLOPT_URL => sprintf(
        'https://' . $this->domain . '.deployhq.com/%s', trim($path, '/')
      ),
      CURLOPT_USERPWD => sprintf(
        '%s:%s', $this->user, $this->key
      ),
      CURLOPT_RETURNTRANSFER => true,
      CURLOPT_HEADER => false,
      CURLOPT_HTTPHEADER => array(
        'Accept: application/json',
        'Content-type: application/json'
      )
    ));

    if ($data !== null) {
      curl_setopt($this->curl, CURLOPT_POSTFIELDS, $data);
    }

    $data = trim(curl_exec($this->curl));
    $url = curl_getinfo($this->curl, CURLINFO_EFFECTIVE_URL);
    $status = curl_getinfo($this->curl, CURLINFO_HTTP_CODE);

    if (!empty($data)) {
      $data = json_decode($data);
    }

    switch ($status) {
      case 200:
      case 201:
        return new SamuraiDeployHQSuccessResponse($status, $url, $data);
      default:
        throw new SamuraiDeployHQErrorResponse($status, $url, $data);
    }
  }
}

class SamuraiDeployHQSuccessResponse {
  public $document;
  public $url;
  public $status;

  public function __construct($status, $url,  $document) {
    $this->document = $document;
    $this->url = $url;
    $this->status = $status;
  }
}

class SamuraiDeployHQErrorResponse extends Exception {
  public $document;
  public $url;
  public $status;

  public function __construct($status, $url,  $document) {
    $this->document = $document;
    $this->url = $url;
    $this->status = $status;
    parent::__construct(sprintf("Error occured while trying to access '%s'.", $url));
  }
}
