<?php
/**
 * @file
 * Enables modules and site configuration for the Samurai site installation.
 */

// Initalise the site_install_hooks
if (!function_exists('site_install_hooks_initialize')) {
  require_once('libraries/site_install_hooks/site_install_hooks.inc');
}
site_install_hooks_initialize('samurai');

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function samurai_form_install_configure_form_alter(&$form, $form_state) {

  // Pre-populate the site name with the profile name.
  $form['site_information']['site_name']['#default_value'] = 'Samurai';
}

/**
 * Implements hook_install_tasks()
 */
function samurai_install_tasks(&$install_state) {

  $task = array();

  // Add a new form step.
  $task['samurai_config'] = array(
    'display_name' => st('Samurai configuration'),
    'display' => TRUE,
    'type' => 'form',
    'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
    'function' => 'samurai_config_form',
  );

  // Add a step to create the default image
  $task['samurai_create_default_image'] = array(
    'display_name' => st('Create default image'),
    'display' => TRUE,
    'type' => 'form',
    'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
    'function' => 'samurai_create_default_image_form',
  );

  return $task;
}

/**
 * Form handler samurai_config_form()
 */
function samurai_config_form($form, &$form_state) {

  $form = array();

  // General information
  $form['general_information'] = array(
    '#type' => 'markup',
    '#markup' => t('Each of these values can be reconfigured in the administration interface'),
  );
  // Wrapper for Docker form elements.
  $form['docker_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Docker configuration'),
    '#description' => t('Configuration values used for managing Docker containers.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // Maximum active docker containers.
  $form['docker_fieldset']['docker_max_active_containers'] = array(
    '#type' => 'select',
    '#title' => t('Maximum active containers'),
    '#description' => t('The maximum number of containers that can be running at any one time - uses system resources.'),
    '#required' => TRUE,
    '#options' => range(1, 50),
  );
  // Maximum inactive docker containers.
  $form['docker_fieldset']['docker_max_inactive_containers'] = array(
    '#type' => 'select',
    '#title' => t('Maximum inactive containers'),
    '#description' => t('The maximum number of inactive containers that can be left on the system before the overflow being deleted. 0 for unlimited.'),
    '#options' => range(0, 100),
    '#required' => TRUE,
  );
  // Number of days to leave an inactive container before deletion.
  $form['docker_fieldset']['docker_max_inactive_days'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of days before deleting an inactive container'),
    '#description' => t('The snumber of days before deleting an inactive docker container that has been inactive for a specified number of days. 0 for unlimited.'),
    '#options' => range(0, 60),
    '#required' => TRUE,
  );
  // Site configuration wrapper
  $form['site_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Interval configuration'),
    '#description' => t('Configure values for regarding update checking intervals'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // Check the site for new updates.
  $form['site_wrapper']['site_check_interval'] = array(
    '#type' => 'select',
    '#title' => t('Interval to scan a site'),
    '#description' => t('Scan a site at an interval for module updates. Recommended 2 days.'),
    '#options' => getIntervalOptions(),
    '#required' => TRUE,
  );
  $form['site_wrapper']['module_check_interval'] = array(
    '#type' => 'select',
    '#title' => t('Interval to check for general module updates'),
    '#description' => t('Interval to check a module for a new version, this is used to indicate you when a module becomes out of date. Recommended 1 day.'),
    '#options' => getIntervalOptions(),
    '#required' => TRUE,
  );
  $form['site_wrapper']['security_interval'] = array(
    '#type' => 'select',
    '#title' => t('Interval to check for security updates'),
    '#description' => t('Interval to check for a security update, upon a security announcement affecting an attached site, samurai will initiate the update process. Recommended 5 mins'),
    '#options' => getIntervalOptions(),
    '#required' => TRUE,
  );
  // Wrapper for GitAPI form elements.
  $form['git_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('GitAPI configuration'),
    '#description' => t('Configuration values required for performing actions with GitAPI.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // GitAPI private key field.
  $form['git_fieldset']['gitapi_privkey'] = array(
    '#type' => 'textarea',
    '#title' => t('SSH private key'),
    '#description' => t('An SSH private key, used for cloning repositories in to Docker containers.'),
    '#required' => TRUE
  );
  // Form submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save and continue',
  );

  return $form;
}

/**
 * Form validation for samurai_config_form()
 */
function samurai_config_form_validate($form, $form_state) {

  // Check the private key is a valid private key

}

/**
 * Form submit handler for samurai_config_form()
 */
function samurai_config_form_submit($form, $form_state) {

  // Set the variables

  // security_samurai_docker
  variable_set('samurai_docker_max_active_containers', $form_state['values']['docker_max_active_containers']);
  variable_set('samurai_docker_max_inactive_containers', $form_state['values']['docker_max_inactive_containers']);
  variable_set('samurai_docker_max_inactive_days', $form_state['values']['docker_max_inactive_days']);

  // security_samurai_controller
  variable_set('samurai_controller_site_check_interval', strtotime($form_state['values']['site_check_interval']));
  variable_set('samurai_controller_module_check_interval', strtotime($form_state['values']['module_check_interval']));
  variable_set('samurai_controller_sa_check_interval', strtotime($form_state['values']['security_interval']));

  // security_samurai_gitapi
  variable_set('samurai_gitapi_privkey', encrypt($form_state['values']['gitapi_privkey']));

  // Rebuild the menu cache
  menu_rebuild();

  // Rebuild theme caches so things are right
  system_rebuild_theme_data();
  drupal_theme_rebuild();
}

/**
 * Form handler samurai_create_default_image_form()
 */
function samurai_create_default_image_form($form, &$form_state) {

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
    '#markup' => t('Loading data...'),
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


  // From here there nneeds to be a markup button which initialises the creation of the image,
  // the build log is then outputted on to the screen

  // IF the build fails, allow the user to start again/provide instructions for a manual setup.

  return $form;
}

/**
 * Helper function, returns an array of intervals
 * @see samurai_config_form()
 */
function getIntervalOptions() {

  // Return the array of intervals
  return array(
    0 => '5 minutes',
    1 => '30 minutes',
    2 => '1 hour',
    3 => '3 hours',
    4 => '6 hours',
    5 => '12 hours',
    6 => '1 day',
    7 => '2 days',
    8 => '4 days',
    9 => '1 week',
    10 => '1 month',
  );
}
