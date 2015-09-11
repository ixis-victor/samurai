<?php
/**
 * @file
 * security_samurai_gitapi.profile.inc
 * Profile install functions
 */

/**
 * Returns a form for configuring GitAPI values
 */
function security_samurai_gitapi_profile_config_form($form, &$form_state) {

  $form = array();

  // General information
  $form['general_information'] = array(
    '#type' => 'markup',
    '#markup' => t('Each of these values can be reconfigured in the administration interface'),
  );
  // Wrapper for GitAPI form elements.
  $form['git_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('GitAPI configuration'),
    '#description' => t('Configuration values required for performing actions with GitAPI.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // Codebase options
  $form['git_fieldset']['gitapi_codebase_info'] = array(
    '#type' => 'markup',
    '#markup' => t('Codebase access'),
    '#prefix' => '<hr/><label class="fieldset-legend" style="padding-left: 0; position: relative">',
    '#suffix' => '</label>'
  );
  $form['git_fieldset']['gitapi_codebase_user'] = array(
    '#type' => 'textfield',
    '#title' => t('API Username'),
    '#description' => t('Your Codebase API username, found next to the API key.'),
    '#required' => TRUE
  );
  $form['git_fieldset']['gitapi_codebase_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('The API key located on your account settings page. This field will remain blank even when configured - for your security.'),
    '#required' => TRUE
  );
  $form['git_fieldset']['gitapi_hr'] = array(
    '#type' => 'markup',
    '#markup' => '<hr/>',
  );
  // GitAPI private key field.
  $form['git_fieldset']['gitapi_privkey'] = array(
    '#type' => 'textarea',
    '#title' => t('SSH private key'),
    '#description' => t('An SSH private key, used for cloning repositories in to Docker containers.'),
    '#required' => TRUE
  );

  // Submit button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and continue'),
  );

  return $form;
}

/**
 * Form validation handler
 * @see security_samurai_gitapi_profile_config_form
 */
function security_samurai_gitapi_profile_config_form_validate($form, $form_state) {

  // Check gitapi_privkey is a valid private key
}

/**
 * Form submit handler
 * @see security_samurai_gitapi_profile_config_form
 */
function security_samurai_gitapi_profile_config_form_submit($form, $form_state) {

  // Set the variables
  variable_set('samurai_gitapi_privkey', encrypt($form_state['values']['gitapi_privkey']));

  // Codebase usernmae and apikey
  $codebase_user = $form_state['values']['gitapi_codebase_user'];
  $codebase_apikey = $form_state['values']['gitapi_codebase_api_key'];
  variable_set('security_samurai_gitapi_codebase_user', $codebase_user);
  variable_set('security_samurai_gitapi_codebase_api_key', encrypt($codebase_apikey));
}
