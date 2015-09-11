<?php
/**
 * @file
 * security_samurai_docker.profile.inc
 * Profile install functions
 */

/**
 * Returns form for Docker configuration
 */
function security_samurai_docker_profile_config_form($form, &$form_state) {

  $form = array();

  // General information
  $form['general_information'] = array(
    '#type' => 'markup',
    '#markup' => t('Each of these values can be reconfigured in the administration interface'),
  );
  // Wrapper for Docker form elements.
  $form['docker_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Environment configuration'),
    '#description' => t('Configuration values used for managing Docker containers.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // Maximum active docker containers.
  $form['docker_fieldset']['docker_max_active_containers'] = array(
    '#type' => 'select',
    '#title' => t('Maximum active environments'),
    '#description' => t('The maximum number of containers that can be running at any one time - uses system resources.'),
    '#required' => TRUE,
    '#options' => range(1, 50),
  );
  // Number of days to leave an inactive container before deletion.
  $form['docker_fieldset']['docker_max_inactive_days'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of days before deleting an inactive environment'),
    '#description' => t('The snumber of days before deleting an inactive docker container that has been inactive for a specified number of days. 0 for unlimited.'),
    '#options' => range(0, 60),
    '#required' => TRUE,
  );
  // Form submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save and continue',
  );

  return $form;
}

/**
 * Form validation handler
 * @see security_samurai_docker_profile_config_form
 */
function security_samurai_docker_profile_config_form_validate($form, $form_state) {

  //
}

/**
 * Form submit handler
 * @see security_samurai_docker_profile_config_form
 */
function security_samurai_docker_profile_config_form_submit($form, $form_state) {

  // Set the variables
  variable_set('samurai_docker_max_active_containers', $form_state['values']['docker_max_active_containers']);
  variable_set('samurai_docker_max_inactive_days', $form_state['values']['docker_max_inactive_days']);
}

/**
 * Returns form for creating the default docker image
 *
 * No validate or submit handler is required for this form
 */
function security_samurai_docker_default_image_form($form, &$form_state) {

  // Set the dockerfile location for the default image.
  $dockerfile_location = drupal_get_path('module', 'security_samurai_docker') . '/docker/images/default/.';
  variable_set('security_samurai_default_dockerfile_location', $dockerfile_location);

  $form = array();

  // Simple markup for the title and info.
  $form['title'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>' . t('Create default image') . '</h2><p>Ensure you have the <a href="https://docs.docker.com" target="_blank">Docker CLI</a> installed and set up correctly on your server.</p><p>This process may take up to 5 minutes.</p>',
  );
  // Attach the Ajax file
  $form['#attached']['js'] = array(
    drupal_get_path('profile', 'samurai') . '/samurai-ajax.js',
  );
  // Attach ajax libraries
  $form['#attached']['library'] = array(
    array('system', 'drupal.ajax'),
  );
  // Container
  $form['container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'samurai-remote-wrapper',
      'title' => t('Click to create image.'),
    ),
  );
  // Container content
  $form['container']['content'] = array(
    '#prefix' => '<pre>',
    '#suffix' => '</pre>',
    '#markup' => t('Creating image, please wait...'),
  );
  // Submit button will remain hidden while the build is in progress
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and continue'),
    '#attributes' => array(
      'style' => array(
        'display: none',
      ),
    ),
  );

  return $form;
}
