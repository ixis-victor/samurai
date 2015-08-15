<?php
/**
 * @file security_samurai_gitapi.admin.inc
 * Administration/Configuration functions, forms.
 */

/**
 * Implements hook_form
 */
function security_samurai_gitapi_admin_config_form($node, &$form_state) {

	$form = array();

	$form['security_samurai_gitapi_config_message'] = array(
		'#type' => 'markup',
		'#markup' => t('The following form provides integration between Security Samurai and your Git repositories.'),
	);

	// Bitbucket options
	$form['security_samurai_gitapi_bitbucket_wrapper'] = array(
		'#type' => 'fieldset',
		'#title' => t('Bitbucket'),
		'#description' => t('Please provide your password and username for Bitbucket (password will be encrypted).'),
		'#collapsible' => TRUE,
	);

	$form['security_samurai_gitapi_bitbucket_wrapper']['security_samurai_gitapi_bitbucket_user'] = array(
		'#type' => 'textfield',
		'#title' => t('Username'),
		'#description' => t('Your Bitbucket username'),
		'#default_value' => variable_get('security_samurai_gitapi_bitbucket_user', ''),
	);

	$form['security_samurai_gitapi_bitbucket_wrapper']['security_samurai_gitapi_bitbucket_pass'] = array(
		'#type' => 'password',
		'#title' => t('Password'),
		'#description' => t('Your Bitbucket password'),
	);

	// Codebase options
	$form['security_samurai_gitapi_codebase_wrapper'] = array(
		'#type' => 'fieldset',
		'#title' => t('Codebase'),
		'#description' => t('Please provide your password and username for Codebase (password will be encrypted).'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

  $form['security_samurai_gitapi_codebase_wrapper']['security_samurai_gitapi_codebase_user'] = array(
    '#type' => 'textfield',
    '#title' => t('API Username'),
    '#description' => t('Your Codebase API username, found next to the API key.'),
    '#default_value' => variable_get('security_samruai_gitapi_codebase_user', ''),
  );

  $form['security_samurai_gitapi_codebase_wrapper']['security_samurai_gitapi_codebase_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('The API key located on your account settings page. This field will remain blank even when configured - for your security.'),
    '#default_value' => decrypt(variable_get('security_samurai_gitapi_codebase_api_key', '')),
  );

	// GitHub options
	$form['security_samurai_gitapi_github_wrapper'] = array(
		'#type' => 'fieldset',
		'#title' => t('GitHub'),
		'#description' => t('Please provide your password and username for GitHub'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['security_samurai_gitapi_github_wrapper']['security_samurai_gitapi_github_user'] = array(
		'#type' => 'textfield',
		'#title' => t('Username'),
		'#description' => t('Your GitHub username'),
		'#default_value' => variable_get('security_samurai_gitapi_github_user', ''),
	);

	$form['security_samurai_gitapi_github_wrapper']['security_samurai_gitapi_github_pass'] = array(
		'#type' => 'textfield',
		'#title' => t('Password'),
		'#description' => t('Your GitHub password'),
    '#default_value' => variable_get('security_samurai_gitapi_github_pass', ''),
	);

	// Apply a default 'save' button and return the form.
	$form = system_settings_form($form);
  unset($form['#submit']);
	$form['#submit'][] = 'security_samurai_gitapi_admin_config_form_submit';
	return $form;
}


/**
 * Implements hook_form_submit
 */
function security_samurai_gitapi_admin_config_form_submit($form_id, &$form_state) {
	
	// Get the passwords
	$bitbucket_pass = $form_state['values']['security_samurai_gitapi_bitbucket_pass'];
  $github_pass = '';

	// Get the usernames
	$bitbucket_user = $form_state['values']['security_samurai_gitapi_bitbucket_user'];
	$codebase_user = $form_state['values']['security_samurai_gitapi_codebase_user'];
	$github_user = $form_state['values']['security_samurai_gitapi_github_user'];

  // API keys
  $codebase_apikey = $form_state['values']['security_samurai_gitapi_codebase_api_key'];

	// Check if the bitbucket pass has a value.
	if (!empty($bitbucket_pass)) {

		// Set Drupal variable for bitbucket user.
		variable_set('security_samurai_gitapi_bitbucket_user', $bitbucket_user);

		// Encrypt the password and set as variable.
		variable_set('security_samurai_gitapi_bitbucket_pass', encrypt($bitbucket_pass));

	}

  if (!empty($codebase_apikey)) {

    // Set the Drupal variable for codebase user.
    variable_set('security_samruai_gitapi_codebase_user', $codebase_user);

    // Encrypt the password and set as variable.
    variable_set('security_samurai_gitapi_codebase_api_key', encrypt($codebase_apikey));

    // Create a new samurai_codebase object.
    // $samurai_codebase = new samurai_codebase($codebase_user, $codebase_apikey);
    // $reply = $samurai_codebase->get
  }

  if (!empty($github_apikey)) {

    // Set the Drupal variable for github
  }

  drupal_set_message('Settings have been saved.');
}
