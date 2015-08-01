<?php

/**
 * @class Samurai_bitbucket
 * Manage SSH keys and repos automatically.
 */
class samurai_bitbucket {

  // Define the class variables.
  private $password = '';
  private $username = '';
  private $http = NULL;
  private $url = '';
  private $headers = array();
  private $cookies = array();
  private $verify_ssl = FALSE;

  // Set to TRUE to enable verbose logging of curl messages.
  private $debug_verbose = FALSE;

  // Use this as user agent string.
  private $user_agent = 'Mozilla/5.0';

  /**
   * Class constructor
   *
   * Parameters are only required for actions that are 
   * on an account or private repositories.
   *
   * @param string $username The username.
   * @param string $password The encrypted password.
   */
  public function __construct($username, $password) {
    
    // Initialise Curl.
    $this->http = curl_init();
    $this->url = 'https://bitbucket.org/api/';

    // Username and password is not necessary for some functions.
    $this->password = $password;
    $this->username = $username;
  }

  public function curl($url, $method) {
      
    // Define the curl options.
    curl_setopt($this->http, CURLOPT_URL, $this->url . $url);
    curl_setopt($this->http, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($this->http, CURLOPT_HEADER, FALSE);
    curl_setopt($this->http, CURLOPT_SSL_VERIFYPEER, $this->verify_ssl);
    curl_setopt($this->http, CURLOPT_USERAGENT, $this->user_agent);
    curl_setopt($this->http, CURLOPT_VERBOSE, $this->debug_verbose);
    curl_setopt($this->http, CURLOPT_USERPWD, $this->username . ':' . $this->password);


    // Define the switch method.
    switch ($method) {

      case 'GET':

        // Set the GET header and execute the request.
        curl_setopt($this->http, CURLOPT_HTTPGET, TRUE);
        $content = curl_exec($this->http);
        $response = curl_getinfo($this->http);

        // Return the result in a clean array.
        return array(
          'content' => $content,
          'response' => $response,
        );

        break;

      case 'PUT':
        curl_setopt($this->http, CURLOPT_PUT, TRUE);
        break;

      case 'DELETE':
        curl_setopt($this->http, CURLOPT_DELETE, TRUE);
        break;

      case 'POST':
          
        // Set the POST header and execute the request.
        curl_setopt($this->http, CURLOPT_POST, TRUE);
        $content = curl_exec($this->http);

        break;
    }
  }

  /**
   * Add SSH Key
   *
   * @param string $key_string The public key string.
   * @param string $key_identifier The name to give the key.
   */
  public function addSshKey($key_string, $key_identifier) {

    // Not yet implemented
  }

  /**
   * Get SSH Keys
   */
  public function getSshKeys() {

    // - Create the curl request.
    // - Return the content and the response.
    $content = $this->curl('1.0/users/' . $this->username . '/ssh-keys', 'GET');
    return $content;
  }

  /**
   * Get Branches
   *
   * @param string $repo_account The account that manages the repo.
   * @param string $repo_name The name of the repo.
   */
  public function getBranches($repo_account, $repo_name) {

    // - Create the curl request.
    // - Return the content and the response.
    $content = $this->curl('1.0/repositories/' . $repo_account . '/' . $repo_name . '/branches/', 'GET');
    return $content;
  }

}
