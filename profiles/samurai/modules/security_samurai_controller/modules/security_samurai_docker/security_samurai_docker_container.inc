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

    case 'docker_container_form_step_2':
      return 'docker_container_form_step_3'
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
    '#value' => t('Next'),
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
 * Form step 3 for docker_container_form
 */
function docker_container_form_step_3($form, $form_state) {

  // This could be done in docker_container_form_step_2_submit
  $sites_array = samurai_get_sites();
  $site_key = $form_state['multistep_values']['docker_container_form_step_1']['docker_container_form_site'] - 1;
  $site_id = samurai_get_site_id($sites_array[$site_key]);
  $branches_array = gitinfo_branches($site_id);
  $selected_branch = $branches_array[$form_state['multistep_values']['docker_container_form_step_2']['docker_container_form_branch'] - 1];

  // Container information array.
  $container_information = array(
    'site_id' => $site_id,
    'branch' => $selected_branch,
    'image' => 'default',
  );

  // Set the container information temp variable
  variable_set('samurai_container_temp_information', $container_information);

  $form = array();

  // Wrapper for the form elements.
  $form['docker_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Setup options'),
    '#fieldset_icon' => 'settings',
    '#collapsed' => FALSE,
  );
  // Attach custom ajax.
  $form['#attached']['js'] = array(
    drupal_get_path('profile', 'security_samurai_docker') . '/samurai-setup-container-ajax.js',
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
    '#value' => t('Submit'),
    '#attributes' => array(
      'style' => array(
        'display: none',
      ),
    ),
  );

  return $form;
}

/**
 * Form submit handler for docker_container_form
 */
function docker_container_form_submit($form, &$form_state) {

  switch ($form_state['step']) {

    case 'docker_container_form_step_3':
      $form_state['multistep_values'][$form_state['step']] = $form_state['values'];
      if ($form_state['triggering_element']['#value'] != 'Back') {
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

  if (isset($form_state['complete'])) drupal_goto('/jobs/environments');

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
