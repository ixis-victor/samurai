<?php
/**
 * @file
 * security_samurai_docker_container.inc
 * Configuration and management forms for the docker_container entity.
 */

/**
 * Implements drupal_form()
 */
function docker_container_form($form, &$form_state) {

	// If no current step is defined, set it to the first one.
  if (!isset($form_state['step'])) $form_state['step'] = 'docker_container_form_step_1';

  $form = array();

  switch ($form_state['step']) {

    case 'docker_container_form_step_1':
      return docker_container_form_step_1($form, $form_state);
      break;
    case 'docker_container_form_step_2':
      return docker_container_form_step_2($form, $form_state);
      break;
    default:
      return docker_container_form_step_1($form, $form_state);
      break;
  }
}

/**
 * Implements hook_validate
 */
function docker_container_form_validate($form, &$form_state) {

  if ($form_state['triggering_element']['#value'] == 'Back') {
    return;
  }

  switch ($form_state['step']) {

    case 'docker_container_form_step_1':
      return docker_container_form_step_1_validate($form, $form_state);
      break;
    case 'docker_container_form_step_2':
      return docker_container_form_step_2_validate($form, $form_state);
      break;
  }
}

/**
 * Return the next step function name
 */
function docker_container_form_move_to_next($form, $form_state) {

  switch ($form_state['step']) {

    case 'docker_container_form_step_1':
      return 'docker_container_form_step_2';
      break;
  }
}

/**
 * Return the previous step function name
 */
function docker_container_form_move_to_previous($form, $form_state) {

  switch ($form_state['step']) {

    case 'docker_container_form_step_2':
      return 'docker_container_form_step_1';
      break;
  }
}

/**
 * Form step 1 for docker_container_form
 */
function docker_container_form_step_1($form, $form_state) {

  $form = array();

  // Wrapper for the form elements.
  $form['docker_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic environment information'),
    '#fieldset_icon' => 'settings',
    '#collapsed' => FALSE,
  );
  // The container name
  $form['docker_container']['docker_container_form_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Environment name'),
    '#description' => t('The container name, so people know what it\'s being used for.'),
    '#required' => TRUE,
  );
  // The container description
  $form['docker_container']['docker_container_form_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Environment description'),
    '#description' => t('Give a little bit more information about what this environment is going to be used for. (optional)')
  );
  // The site the container will use.
  $sites_array = samurai_get_sites();
  $new_site_array = array();
  array_push($new_site_array, '- Select one -');
  if (!empty($sites_array)) {
    foreach ($sites_array as $key => $value) {
      array_push($new_site_array, $value);
    }
  }
  $form['docker_container']['docker_container_form_site'] = array(
    '#type' => 'select',
    '#title' => t('Site to use'),
    '#description' => t('The site to set up in the environment.'),
    '#options' => $new_site_array,
  );
  // The form next button.
  $form['form_next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

/**
 * Form step 1 validation for docker_container_form
 */
function docker_container_form_step_1_validate($form, $form_state) {

  // Site select cannot be 0
  if ($form_state['values']['docker_container_form_site'] == 0) {
    return form_set_error('docker_container_form_site', 'Select a site to use');
  }
}

/**
 * Form step 2 for docker_container_form
 */
function docker_container_form_step_2($form, $form_state) {

  $form = array();

  // Wrapper for the form elements.
  $form['docker_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Setup options'),
    '#fieldset_icon' => 'settings',
    '#collapsed' => FALSE,
  );
  // Select the branch.
  $site_array = samurai_get_sites();
  $site_key = $form_state['multistep_values']['docker_container_form_step_1']['docker_container_form_site'];
  $branches = gitinfo_branches(samurai_get_site_id($site_array[$site_key - 1]));
  $new_branch_array = array();
  array_push($new_branch_array, '- Select one -');
  foreach ($branches as $key => $value) {
    array_push($new_branch_array, $value);
  }
  $form['docker_container']['docker_container_form_branch'] = array(
    '#type' => 'select',
    '#title' => t('Branch to use'),
    '#description' => t('The branch to use for the site.'),
    '#options' => $new_branch_array,
  );
  // The form back button.
  $form['form_back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
  );
  // The form submit button.
  $form['form_next'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Form step 2 validation for docker_container_form
 */
function docker_container_form_step_2_validate($form, $form_state) {

  //  Branch select cannot be 0
  if ($form_state['values']['docker_container_form_branch'] == 0) {
    return form_set_error('docker_container_form_branch', 'Select a branch to use');
  }
}

/**
 * Form submit handler for docker_container_form
 */
function docker_container_form_submit($form, &$form_state) {

  switch ($form_state['step']) {

    case 'docker_container_form_step_2':
      $form_state['multistep_values'][$form_state['step']] = $form_state['values'];
      if ($form_state['triggering_element']['#value'] != 'Back') {
        docker_container_form_submit_details($form, $form_state);
        $form_state['complete'] = TRUE;
      }
      break;
    default:
      $form_state['multistep_values'][$form_state['step']] = $form_state['values'];
      $form_state['new_step'] = docker_container_form_move_to_next($form, $form_state);
      if (isset($form_state['multistep_values'][$form_state['new_step']])) {
        $form_state['values'] = $form_state['multistep_values'][$form_state['new_step']];
      }
      break;
  }

  if (isset($form_state['complete'])) drupal_goto('/jobs/containers/config');

  if ($form_state['triggering_element']['#value'] == 'Back') {
    $form_state['new_step'] = docker_container_form_move_to_previous($form, $form_state);
    $form_state['values'] = $form_state['multistep_values'][$form_state['new_step']];
  }

  if (isset($form_state['multistep_values']['form_build_id'])) {
    $form_state['values']['form_build_id'] = $form_state['multistep_values']['form_build_id'];
  }
  $form_state['multistep_values']['form_build_id'] = $form_state['values']['form_build_id'];
  $form_state['step'] = $form_state['new_step'];
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit the container creation form details.
 */
function docker_container_form_submit_details($form, $form_state) {

  // Create a new container
  // Use the default image for the container if no image was selected.
  $docker = new Docker;
  $docker->create_container('3000:80', 'default');

  // Clone the repo of the site, grab a database (drush aliases)
  // This process will be different if the site is a make installation or standard.

  // - Get all of the sites.
  // - Get the selected site key.
  // - Get the site ID.
  // - Get all of the branches associated with the selected site.
  // - Get the selected branch.
  $sites_array = samurai_get_sites();
  $site_key = $form_state['multistep_values']['docker_container_form_step_1']['docker_container_form_site'] - 1;
  $site_id = samurai_get_site_id($sites_array[$site_key]);
  $branches_array = gitinfo_branches($site_id);
  $selected_branch = $branches_array[$form_state['multistep_values']['docker_container_form_step_2']['docker_container_form_branch'] - 1];

  // Create the private key file
  $private_key = variable_get('docker_container_form', NULL);
  if (!is_null($private_key)) {

    // Decrypt the private key
    $private_key = decrypt($private_key);
    
    // Add the key to the container
    $docker->execute_commmand('./samurai-scripts/set_ssh_key.sh \'$private_key\'');

    // Clone the site into the www directory
  }

  // Start apache on the container
  $docker->execute_command('service apache2 start');

  // Create a new docker container entity and save it.
  $container = entity_create('docker_container', array(
    'container_id' => $docker->container_id,
    'image_name' => $docker->image_name,
    'assigned_ports' => $docker->assigned_ports,
    'status' => 1,
    'creation_date' => REQUEST_TIME,
  ));
  $container_wrapper = entity_metadata_wrapper('docker_container', $container);
  $container_wrapper->save();
}
