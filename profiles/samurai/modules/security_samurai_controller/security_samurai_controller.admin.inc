<?php
/**
 * @file
 * Administrative UI functions.
 *
 * These functions handle creating a new entity
 * entity type of 'site'
 */

/**
 * Implements hook_form
 */
function client_site_form($form, &$form_state, $site = NULL) {


  // If no current step is defined, set it to the first one.
  if (!isset($form_state['step']))
    $form_state['step'] = 'client_site_form_step_1';

  $form = array();
  switch ($form_state['step']) {
    case 'client_site_form_step_1':
      return client_site_form_step_1($form, $form_state, $site);
      break;
    case 'client_site_form_step_2':
      return client_site_form_step_2($form, $form_state);
      break;
    case 'client_site_form_step_3':
      return client_site_form_step_3($form, $form_state);
      break;
    case 'client_site_form_step_4':
      return client_site_form_step_4($form, $form_state);
      break;
    case 'client_site_form_step_5':
      return client_site_form_step_5($form, $form_state);
      break;
    default:
      return client_site_form_step_1($form, $form_state);
      break;
  }
}

/**
 * Return the next step function name
 */
function client_site_form_move_to_next($form, $form_state) {

  switch ($form_state['step']) {
    case 'client_site_form_step_1':
      return 'client_site_form_step_2';
      break;
    case 'client_site_form_step_2':
      return 'client_site_form_step_3';
      break;
    case 'client_site_form_step_3':
      return 'client_site_form_step_4';
      break;
    case 'client_site_form_step_4':
      return 'client_site_form_step_5';
      break;
  }
}

/**
 * Return the previous step function name
 */
function client_site_form_move_to_previous($form, $form_state) {

  switch ($form_state['step']) {
    case 'client_site_form_step_5':
      return 'client_site_form_step_4';
      break;
    case 'client_site_form_step_4':
      return 'client_site_form_step_3';
      break;
    case 'client_site_form_step_3':
      return 'client_site_form_step_2';
      break;
    case 'client_site_form_step_2':
      return 'client_site_form_step_1';
      break;
  }
}

/**
 * Implements hook_validate
 */
function client_site_form_validate($form, &$form_state) {

  if ($form_state['triggering_element']['#value'] == 'Back') {
    return;
  }
  switch ($form_state['step']) {
    case 'client_site_form_step_1':
      return client_site_form_step_1_validate($form, $form_state);
      break;
    case 'client_site_form_step_2':
      return client_site_form_step_2_validate($form, $form_state);
      break;
    // No validation for step 3 needed
    case 'client_site_form_step_4':
      return client_site_form_step_4_validate($form, $form_state);
      break;
    // No validation for step 5 needed
  }
}

/**
 * The client_site_form form submit handler
 */
function client_site_form_submit($form, &$form_state) {

  switch ($form_state['step']) {
    case 'client_site_form_step_5':
      $form_state['multistep_values'][$form_state['step']] = $form_state['values'];
      if ($form_state['triggering_element']['#value'] != 'Back') {
        client_site_form_submit_details($form, $form_state);
        $form_state['complete'] = TRUE;
      }
      break;
    default:
      $form_state['multistep_values'][$form_state['step']] = $form_state['values'];
      $form_state['new_step'] = client_site_form_move_to_next($form, $form_state);
      if (isset($form_state['multistep_values'][$form_state['new_step']])) {
        $form_state['values'] = $form_state['multistep_values'][$form_state['new_step']];
      }
      break;
  }
  // If the form is complete, redirect to the site_id
  if (isset($form_state['complete']))
    drupal_goto('/client-site/' . samurai_get_site_id($form_state['multistep_values']['client_site_form_step_1']['name']));
  // If back submit button is selected
  if ($form_state['triggering_element']['#value'] == 'Back') {
    $form_state['new_step'] = client_site_form_move_to_previous($form, $form_state);
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
 * Call the submit handlers for each step.
 */
function client_site_form_submit_details($form, $form_state) {

  // Submit step 1
  $form_state['values'] = $form_state['multistep_values']['client_site_form_step_1'];
  client_site_form_submit_details_step_1($form, $form_state);
  // Submit step 2
  $form_state['values'] = $form_state['multistep_values']['client_site_form_step_2'];
  client_site_form_submit_details_step_2($form, $form_state);
  // Submit step 3
  $form_state['values'] = $form_state['multistep_values']['client_site_form_step_3'];
  client_site_form_submit_details_step_3($form, $form_state);
  // Submit step 4
  $form_state['values'] = $form_state['multistep_values']['client_site_form_step_4'];
  client_site_form_submit_details_step_4($form, $form_state);
  // Submit step 5
  $form_state['values'] = $form_state['multistep_values']['client_site_form_step_5'];
  client_site_form_submit_details_step_5($form, $form_state);
}

/**
 * Client form step 1
 */
function client_site_form_step_1($form, $form_state, $site = NULL) {

  // Set the default value for the name element
  if (isset($site->name)) {
    $name_default = $site->name;
  } else if (isset($form_state['values']['name'])) {
    $name_default = $form_state['values']['name'];
  } else {
    $name_default = '';
  }
  // Set the default value for the encryption_key element
  if (isset($site->encryption_key)) {
    if (strlen($site->encryption_key) > 0) {
      $encryption_default = $site->encryption_key;
    } elseif (isset($form_state['values']['encryption_key'])) {
      $encyption_default = $form_state['values']['encryption_key'];
    }
  } elseif (isset($form_state['values']['encryption_key'])) {
    $encryption_default = $form_state['values']['encryption_key'];
  }
  // Set the default value for the site_alias_id element
  if (isset($site->site_alias_id)) {
    $drush_alias_default = $site->site_alias_id;
  } else if (isset($form_state['values']['drush_alias'])) {
    $drush_alias_default = $form_state['values']['drush_alias'];
  } else {
    $drush_alias_default = 0;
  }
  // Fieldset for the form elements
  $form['client_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site details'),
    '#collapsed' => FALSE,
    '#fieldset_icon' => 'explore',
    '#attributes' => array(
      'class' => array('active'),
    ),
  );
  // The human readable name to give the site.
  $form['client_fieldset']['name'] = array(
    '#title' => t('Site name'),
    '#type' => 'textfield',
    '#default_value' => $name_default,
    '#description' => t('The name of the site.'),
    '#required' => TRUE,
  );
  // The encryption key.
  $form['client_fieldset']['encryption_key'] = array(
    '#title' => t('Encryption key'),
    '#type' => 'textfield',
    '#default_value' => isset($encryption_default) ? $encryption_default : FALSE,
    '#description' => t('The key provided by the site.'),
  );
  // Check if the sites site_url is already an existing value.
  // If it isn't, then the site has not been submitted before and the site_url
  // will be extracted from the enryption_key
  if (isset($site->site_url)) {
    $form['client_fieldset']['site_url'] = array(
      '#title' => t('Site URL'),
      '#type' => 'textfield',
      '#default_value' => isset($site->site_url) ? $site->site_url : '',
      '#description' => t('The site URL, decoded from the encryption key'),
      '#required' => TRUE,
    );
  }
  // DRUSH ALIASES CURRENTLY A WIP FOR SCANNING SITES
  // Markup to make it look nicer
  // $form['client_fieldset']['markup'] = array(
  //   '#type' => 'markup',
  //   '#markup' => '<h1 class="center" style="margin: 0; padding: 0">OR</h1><p class="center" style="margin: 0; padding: 0">In place of the encryption key, you can connect a site directly via drush!</p>',
  // );
  // The drush alias
  $form['client_fieldset']['drush_alias'] = array(
    '#title' => t('Drush alias'),
    '#description' => t('The drush alias used by the site'),
    '#type' => 'select',
    '#options' => array_merge(array(0 => '- Select -'), variable_get('security_samurai_aliases', array(1 => 'No aliases available'))),
    '#default_value' => $drush_alias_default,
  );
  // Default form submit actions.
  $form['form_next'] = array(
    '#type' => 'submit',
    '#value' => 'Next',
  );

  // Return the form
  return $form;
}

/**
 * Form validation handler for the site details step of the client_site_form
 */
function client_site_form_step_1_validate($form, $form_state) {

  // Check if the site doesn't already exist
  if (arg(2) != 'manage') {
    if (samurai_site_exists($form_state['values']['name'])) {
      return form_set_error('name', t('A site with this name already exists'));
    }
  }
  // drush aliases OR encryption key must be set
  // Drush aliases is currently still a WIP
  // if ($form_state['values']['drush_alias'] == 0 && strlen($form_state['values']['encryption_key']) == 0) {
  //   return form_set_error('encryption_key', t('A value must be given for either the Drush alias select or the encryption key textfield'));
  // }
  if (strlen($form_state['values']['encryption_key']) == 0) {
    return form_set_error('encryption_key', t('A value must be given for the encryption key textfield'));
  }
  $encryption_key_length = strlen($form_state['values']['encryption_key']);
  if ($encryption_key_length < 32) {
    return form_set_error('encryption_key', t('Enter a valid encryption key'));
  }
  if (!isset($form_state['values']['site_url'])) {
    // Retrieve the site_url.
    $encryptionsiteurl = $form_state['values']['encryption_key'];
    $site_url = substr($encryptionsiteurl, 32);
    $site_url = base64_decode($site_url);
    // Attempt connection to the site.
    if (!samurai_check_site_access($site_url)) {
      return form_set_error('encryption_key', t('Unable to connect to the site, check the encryption key is correct and
        Samurai is configured properly to access the site'));
    }
  }
  if (isset($form_state['values']['drush_alias'])) {
    // If drush aliases is set
  }
}


/**
 * Form submit handler for the site details step of the client_site_form
 */
function client_site_form_submit_details_step_1($form, $form_state) {

  // Decrypt the site_url from the encryption string.
  if (!isset($form_state['values']['site_url'])) {
    $encryptionsiteurl = $form_state['values']['encryption_key'];
    $site_url = substr($encryptionsiteurl, 32);
    $encryption_string = substr($encryptionsiteurl, 0, 32);
    $form_state['values']['encryption_key'] = $encryption_string;
    $form_state['values']['site_url'] = base64_decode($site_url);
  }
  // Check if the site is being updated or saved.
  if (!samurai_site_exists($form_state['values']['name'], $form_state['values']['site_url'])) {
    // Set the update_next_check to 0, so the updates are checked
    // on the next cron execution - every minute.
    $form_state['values']['update_next_check'] = 0;
  }
  // Retrieve the site alias name
  $site_aliases = variable_get('security_samurai_aliases', NULL);
  $site_alias_name = $site_aliases[$form_state['values']['drush_alias']];
  // Set the update method for the site
  // 1 = encryption key
  // 2 = drush aliases
  $update_check_method = 0;
  if (strlen($form_state['values']['encryption_key']) > 0) {
    $update_check_method = 1;
  } elseif (isset($form_state['values']['drush_alias']) ) {
    $update_check_method = 2;
  }
  // -- Save the entity.
  // -- Print notification messages.
  // -- Log to watchdoge.
  $site_id = arg(3);
  if (!samurai_site_exists($form_state['values']['name']) && is_null($site_id)) {
    // Entity doesn't exist.
    $site = entity_create('client_site', array(
      'name' => $form_state['values']['name'],
      'encryption_key' => $form_state['values']['encryption_key'],
      'site_url' => $form_state['values']['site_url'],
      'update_next_check' => 0,
      'update_check_method' => $update_check_method,
      'site_alias_id' => $form_state['values']['drush_alias'],
      'site_alias_name' => $site_alias_name
    ));
    $site_wrapper = entity_metadata_wrapper('client_site', $site);
    $site_wrapper->save();
  } else {
    // Entity already exists, load it and update it.
    $site = client_site_load($site_id);
    $site->name = $form_state['values']['name'];
    $site->encryption_key = $form_state['values']['encryption_key'];
    $site->site_url = $form_state['values']['site_url'];
    $site->site_alias_id = $form_state['values']['drush_alias'];
    $site->site_alias_name = $site_alias_name;
    $site->update_next_check = 0;
    $site->update_check_method = $update_check_method;
    $site->save();
  }
  drupal_set_message(t('@name has been saved.', array('@name' => $site->name)));
  watchdog('Security samurai', t('@name has been saved.', array('@name' => $site->name)));
}

/**
 * Client form step 2
 *
 * Validation handler @see client_site_form_step_2_validate
 * Submit handler @see client_site_form_submit_details_step_2
 */
function client_site_form_step_2($form, $form_state) {

  // Set the default value for the docker_phpversion element
  if (isset($form_state['values']['docker_phpversion'])) {
    $phpversion_default = $form_state['values']['docker_phpversion'];
  } elseif (isset($docker->phpversion)) {
    // Not yet implemented
  } else {
    $phpversion_default = '';
  }
  // Set the default value for the docker_chosenimage element
  if (isset($form_state['values']['docker_chosenimage'])) {
    $chosenimage_default = $form_state['values']['docker_chosenimage'];
  } elseif (isset($docker->chosenimage)) {
    // Placeholder, will eventually be filled.
  } else {
    $chosenimage_default = '';
  }

  // Container for the docker elements
  $form['docker_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Docker configuration'),
    '#description' => t('Docker is used for creating test environments when applying the security updates.
      The below configuration options are custom to this client.'),
    '#collapsed' => FALSE,
    '#fieldset_icon' => 'cloud',
  );
  // Select PHP version to run.
  $form['docker_fieldset']['docker_chosenos'] = array(
    '#type' => 'select',
    '#title' => t('Docker container OS'),
    '#description' => t('What operating system would you like the Docker container to use?'),
    '#options' => array(
      'debian:wheezy',
    ),
  );
  // Select for docker PHP version
  $form['docker_fieldset']['docker_phpversion'] = array(
    '#type' => 'select',
    '#title' => t('Docker container PHP version'),
    '#description' => t('What PHP version would you like the Docker container to install when running your site?'),
    '#default_value' => $phpversion_default,
    '#options' => array(
      1 => 'PHP 5.5 latest',
    ),
  );
  // Select for docker image
  $form['docker_fieldset']['docker_chosenimage'] = array(
    '#type' => 'select',
    '#title' => t('Docker image to use'),
    '#description' => t('What docker image would you like the Docker container to use when running your site?'),
    '#default_value' => $chosenimage_default,
    '#options' => array(
      0 => 'default',
    ),
  );
  // Form back button
  $form['form_back'] = array(
    '#type' => 'submit',
    '#value' => 'Back',
  );
  // Form next button
  $form['form_next'] = array(
    '#type' => 'submit',
    '#value' => 'Next',
  );
  // Return the form
  return $form;
}

/**
 * Form validation handler
 * @see client_site_form_step_2
 */
function client_site_form_step_2_validate($form, $form_state) {

  // Check the docker PHP version is selected.
  if ($form_state['values']['docker_phpversion'] == 0) {
    return form_set_error('docker_phpversion', t('Select a PHP version'));
  }
}

/**
 * Form submit handler
 * @see client_site_form_step_2
 */
function client_site_form_submit_details_step_2($form_id, $form_state) {
  // Not yet implemented
}

/**
 * Client form step 3
 *
 * Validation handler @see client_site_form_step_3_validate
 * Submit handler @see client_site_form_submit_details_step_3
 */
function client_site_form_step_3($form, $form_state) {

  // Load the gitinfo entity if one exists
  $site_id = arg(3);
  $gitinfo = NULL;
  if (gitinfo_exists($site_id)) {
    $gitinfo_id = gitinfo_getid($site_id);
    if ($gitinfo_id != FALSE) {
      $gitinfo = gitinfo_load($gitinfo_id);
    }
  }
  // Load the deployinfo entity if exists
  $deployinfo = NULL;
  if (deployinfo_exists($site_id)) {
    $deployinfo_id = deployinfo_getid($site_id);
    if (!is_null($deployinfo_id)) {
      $deployinfo = deployinfo_load($deployinfo_id);
    }
  }
  // Set the default value for the gitapi_service element
  if (isset($form_state['values']['gitapi_service'])) {
    $service_default = $form_state['values']['gitapi_service'];
  } elseif (!is_null($gitinfo)) {
    $service_default = $gitinfo->service;
  } else {
    $service_default = 3;
  }
  // Set the default value for the gitapi_clone_url element
  if (isset($form_state['values']['gitapi_clone_url'])) {
    $clone_default = $form_state['values']['gitapi_clone_url'];
  } elseif (!is_null($gitinfo)) {
    $clone_default = $gitinfo->clone_url;
  } else {
    $clone_default = '';
  }
  // Set the default value for the gitapi_workflow_model element
  if (isset($form_state['values']['gitapi_workflow_model'])) {
    $workflow_model_default = $form_state['values']['gitapi_workflow_model'];
  } elseif (!is_null($gitinfo)) {
    $workflow_model_default = $gitinfo->workflow;
  } else {
    $workflow_model_default = 0;
  }
  // Set the default value for the deploy_service element
  if (isset($form_state['values']['deploy_service'])) {
    $deploy_service_default = $form_state['values']['deploy_service'];
  } elseif (!is_null($deployinfo)) {
    $deploy_service_default = $deployinfo->deploy_service_provider;
  } else {
    $deploy_service_default = 0;
  }
  // Fieldset for the services elements
  $form['services_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Services configuration'),
    '#fieldset_icon' => 'extension',
  );
  // The Git service the repository uses
  $form['services_fieldset']['gitapi_service'] = array(
    '#type' => 'select',
    '#title' => t('Git service'),
    '#description' => t('The Git service to use.'),
    '#required' => TRUE,
    '#options' => array(
      1 => t('GitHub'),
      2 => t('BitBucket'),
      3 => t('Codebase'),
    ),
    '#default_value' => $service_default,
  );
  // Allow a user to choose the preferred Git workflow
  // the branches from this are configured in the next step
  $form['services_fieldset']['gitapi_workflow_model'] = array(
    '#type' => 'select',
    '#title' => t('Git workflow model'),
    '#description' => t('The perferred Git workflow model for this project.'),
    '#required' => TRUE,
    '#options' => array(
      0 => 'Git flow',
      1 => 'Dev -> Stage -> Prod',
      2 => 'Dev -> Prod',
    ),
    '#default_value' => $workflow_model_default,
  );
  // The repository clone URL
  $form['services_fieldset']['gitapi_clone_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Clone URL'),
    '#description' => t('The Git URL to the repo containing the site.'),
    '#default_value' => $clone_default,
    '#required' => TRUE,
  );
  // The deployment service used by the project
  $form['services_fieldset']['deploy_service'] = array(
    '#type' => 'select',
    '#title' => t('Deployment service'),
    '#description' => t('The preferred deployment service for this project.'),
    '#required' => TRUE,
    '#options' => array(
      0 => 'DeployHQ',
    ),
    '#default_value' => $deploy_service_default,
  );
  // Form back button
  $form['form_back'] = array(
    '#type' => 'submit',
    '#value' => 'Back',
  );
  // Form submit button
  $form['form_next'] = array(
    '#type' => 'submit',
    '#value' => 'Next',
  );
  // Return the form
  return $form;
}

/**
 * Form submit handler
 * @see client_site_form_step_3
 */
function client_site_form_submit_details_step_3($form, $form_state) {

}

/**
 * Client form step 4
 *
 * Validation handler @see client_site_form_step_4_validate
 * Submit handler @see client_site_form_submit_details_step_4
 */
function client_site_form_step_4($form, $form_state) {

  // Retrieve the branches
  $gitapi_service = $form_state['multistep_values']['client_site_form_step_3']['gitapi_service'];
  $gitapi_clone_url = $form_state['multistep_values']['client_site_form_step_3']['gitapi_clone_url'];
  $branches = get_branches($gitapi_service, $gitapi_clone_url);
  // Retrieve the workflow model
  $workflow_model = $form_state['multistep_values']['client_site_form_step_3']['gitapi_workflow_model'];
  // Load the gitinfo entity if one exists.
  $site_id = arg(3);
  $gitinfo = NULL;
  // Load the Gitinfo entity if exists
  if (gitinfo_exists($site_id)) {
    $gitinfo_id = gitinfo_getid($site_id);
    if ($gitinfo_id != FALSE) {
      $gitinfo = gitinfo_load($gitinfo_id);
    }
  }
  // Load the deployinfo entity if exists
  $deployinfo = NULL;
  if (deployinfo_exists($site_id)) {
    $deployinfo_id = deployinfo_getid($site_id);
    if (!is_null($deployinfo_id)) {
      $deployinfo = deployinfo_load($deployinfo_id);
    }
  }
  // Set the default value for the gitapi_makefile element
  if (isset($form_state['values']['gitapi_makefile'])) {
    $makefile_default = $form_state['values']['gitapi_makefile'];
  } elseif (!is_null($gitinfo)) {
    $makefile_default = $gitinfo->is_make;
  } else {
    $makefile_default = '';
  }
  // Set the default value for the gitapi_makefile_location
  // element
  if (isset($form_state['values']['gitapi_makefile_location'])) {
    $makefile_location_default = $form_state['values']['gitapi_makefile_location'];
  } elseif (!is_null($gitinfo)) {
    $makefile_location_default = $gitinfo->make_location;
  } else {
    $makefile_location_default = '';
  }
  // Set the default value for the hotfix element
  if (isset($form_state['values']['hotfix'])) {
    $hotfix_default = $form_state['values']['hotfix'];
  } elseif (!is_null($gitinfo)) {
    $workflow_args = drupal_json_decode($gitinfo->workflow_args);
    $hotfix_default = $workflow_args['hotfix'];
  } else {
    $hotfix_default = NULL;
  }
  // Set the default value for the deploy_project element
  if (isset($form_state['values']['deploy_project'])) {
    $deploy_project_default = $form_state['values']['deploy_project'];
  } elseif (!is_null($deployinfo)) {
    $args = drupal_json_decode($deployinfo->args);
    $deploy_project_default = security_samurai_deploy_get_project_key($args['project']);
  } else {
    $deploy_project_default = NULL;
  }
  // Fieldset for the GitAPI elements
  $form['gitapi_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Git repository & workflow information'),
    '#fieldset_icon' => 'info_outline',
    '#collapsed' => FALSE,
  );
  $form['gitapi_fieldset']['deploy_project'] = array(
    '#type' => 'select',
    '#title' => t('Deployment project'),
    '#description' => t('The deploy project this repository corresponds to'),
    '#options' => security_samurai_deploy_retrieve_projects(),
    '#required' => TRUE,
    '#default_value' => $deploy_project_default,
  );
  switch ($workflow_model) {
    // Git flow
    case 0:
      // Set the default value for the master_branch element
      if (isset($form_state['values']['master_branch'])) {
        $master_branch_default = $form_state['values']['master_branch'];
      } elseif (!is_null($gitinfo)) {
        $workflow_args = drupal_json_decode($gitinfo->workflow_args);
        $master_branch_default = $workflow_args['master']['id'];
      } else {
        $master_branch_default = NULL;
      }
      // Set the default value for the develop_branch element
      if (isset($form_state['values']['develop_branch'])) {
        $develop_branch_default = $form_state['values']['develop_branch'];
      } elseif (!is_null($gitinfo)) {
        $workflow_args = drupal_json_decode($gitinfo->workflow_args);
        $develop_branch_default = $workflow_args['develop']['id'];
      } else {
        $develop_branch_default = NULL;
      }
      // Git flow master branch
      $form['gitapi_fieldset']['master_branch'] = array(
        '#type' => 'select',
        '#title' => t('The \'master\' branch'),
        '#description' => t('The equivalent of \'master\' for the Git flow workflow you have selected'),
        '#options' => $branches,
        '#required' => TRUE,
        '#default_value' => $master_branch_default,
      );
      // Git flow develop branch
      $form['gitapi_fieldset']['develop_branch'] = array(
        '#type' => 'select',
        '#title' => t('The \'develop\' branch'),
        '#description' => t('The equivalent of \'develop\' for the Git flow workflow you have selected'),
        '#options' => $branches,
        '#required' => TRUE,
        '#default_value' => $develop_branch_default,
      );
      break;
    // Dev -> stage -> prod
    case 1:
      // Set the default value for the prod_branch element
      if (isset($form_state['values']['prod_branch'])) {
        $prod_branch_default = $form_state['values']['prod_branch'];
      } elseif (!is_null($gitinfo)) {
        $workflow_args = drupal_json_decode($gitinfo->workflow_args);
        $prod_branch_default = $workflow_args['prod']['id'];
      } else {
        $prod_branch_default = NULL;
      }
      // Set the default value for the stage_branch element
      if (isset($form_state['values']['stage_branch'])) {
        $stage_branch_default = $form_state['values']['stage_branch'];
      } elseif (!is_null($gitinfo)) {
        $workflow_args = drupal_json_decode($gitinfo->workflow_args);
        $stage_branch_default = $workflow_args['stage']['id'];
      } else {
        $stage_branch_default = NULL;
      }
      // Set the default value for the develop_branch element
      if (isset($form_state['values']['develop_branch'])) {
        $develop_branch_default = $form_state['values']['develop_branch'];
      } elseif (!is_null($gitinfo)) {
        $workflow_args = drupal_json_decode($gitinfo->workflow_args);
        $develop_branch_default = $workflow_args['dev']['id'];
      } else {
        $develop_branch_default = NULL;
      }
      // prod branch
      $form['gitapi_fieldset']['prod_branch'] = array(
        '#type' => 'select',
        '#title' => t('The \'prod\' branch'),
        '#description' => t('The equivalent of \'prod\' for the Git flow workflow you have selected'),
        '#options' => $branches,
        '#required' => TRUE,
        '#default_value' => $prod_branch_default,
      );
      // stage branch
      $form['gitapi_fieldset']['stage_branch'] = array(
        '#type' => 'select',
        '#title' => t('The \'stage\' branch'),
        '#description' => t('The equivalent of \'stage\' for the Git flow workflow you have selected'),
        '#options' => $branches,
        '#required' => TRUE,
        '#default_value' => $stage_branch_default,
      );
      // dev branch
      $form['gitapi_fieldset']['develop_branch'] = array(
        '#type' => 'select',
        '#title' => t('The \'dev\' branch'),
        '#description' => t('The equivalent of \'dev\' for the Git flow workflow you have selected'),
        '#options' => $branches,
        '#required' => TRUE,
        '#default_value' => $develop_branch_default,
      );
      break;
    // Dev -> prod
    case 2:
      // Set the default value for the prod_branch element
      if (isset($form_state['values']['prod_branch'])) {
        $prod_branch_default = $form_state['values']['prod_branch'];
      } elseif (!is_null($gitinfo)) {
        $workflow_args = drupal_json_decode($gitinfo->workflow_args);
        $prod_branch_default = $workflow_args['prod']['id'];
      } else {
        $prod_branch_default = NULL;
      }
      // Set the default value for the develop_branch element
      if (isset($form_state['values']['develop_branch'])) {
        $develop_branch_default = $form_state['values']['develop_branch'];
      } elseif (!is_null($gitinfo)) {
        $workflow_args = drupal_json_decode($gitinfo->workflow_args);
        $develop_branch_default = $workflow_args['dev']['id'];
      } else {
        $develop_branch_default = NULL;
      }
      // prod branch
      $form['gitapi_fieldset']['prod_branch'] = array(
        '#type' => 'select',
        '#title' => t('The \'prod\' branch'),
        '#description' => t('The equivalent of \'prod\' for the Git flow workflow you have selected'),
        '#options' => $branches,
        '#required' => TRUE,
        '#default_value' => $prod_branch_default,
      );
      // dev branch
      $form['gitapi_fieldset']['develop_branch'] = array(
        '#type' => 'select',
        '#title' => t('The \'dev\' branch'),
        '#description' => t('The equivalent of \'dev\' for the Git flow workflow you have selected'),
        '#options' => $branches,
        '#required' => TRUE,
        '#default_value' => $develop_branch_default,
      );
      break;
  }
  // Git flow hotfix
  $form['gitapi_fieldset']['hotfix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create a hotfix for highly critical security updates'),
    '#default_value' => $hotfix_default,
  );
  // Checkbox for makefile
  $form['gitapi_fieldset']['gitapi_makefile'] = array(
    '#type' => 'checkbox',
    '#title' => t('Does this project depend on a makefile?'),
    '#description' => t('Only select this option if the site set up is based primarily on running a makefile.'),
    '#default_value' => $makefile_default,
  );
  // The location of the makefile in the repository
  $form['gitapi_fieldset']['gitapi_makefile_location'] = array(
    '#type' => 'textfield',
    '#title' => t('Makefile location and name'),
    '#description' => t('The name and location of the makefile to run e.g. some-folder/another-folder/samurai.make'),
    '#attributes' => array(
      'placeholder' => 'make/samurai.make',
    ),
    '#default_value' => $makefile_location_default,
  );
  // Form back button
  $form['form_back'] = array(
    '#type' => 'submit',
    '#value' => 'Back',
  );
  // Form submit button
  $form['form_next'] = array(
    '#type' => 'submit',
    '#value' => 'Next',
  );
  // Return the form
  return $form;
}

/**
 * Form validation handler
 * @see client_site_form_step_4
 */
function client_site_form_step_4_validate($form, $form_state) {

  // Check makefile location is set, if makefile selected
  if ($form_state['values']['gitapi_makefile'] == TRUE &&
   !isset($form_state['values']['gitapi_makefile_location'])) {
    return form_set_error('gitapi_makefile_location', t('The makefile location is required'));
  }
}

/**
 * Form submit handler
 * @see client_site_form_step_4
 */
function client_site_form_submit_details_step_4($form, $form_state) {

  // Retrieve values from previous steps
  $gitapi_service = $form_state['multistep_values']['client_site_form_step_3']['gitapi_service'];
  $gitapi_clone_url = $form_state['multistep_values']['client_site_form_step_3']['gitapi_clone_url'];
  $workflow = $form_state['multistep_values']['client_site_form_step_3']['gitapi_workflow_model'];
  // Retrieve branches from form_state
  $branches = drupal_json_encode(get_branches($gitapi_service, $gitapi_clone_url));

  // Switch through each possible value of workflow
  // setting the correct workflow_args
  $workflow_args = array();
  switch ($workflow) {
    // Git flow
    case 0:
      // Set the args for the master branch
      $workflow_args['master']['string'] = $branches[$form_state['values']['master_branch']];
      $workflow_args['master']['id'] = $form_state['values']['master_branch'];
      // Set the args for the develop branch
      $workflow_args['develop']['string'] = $branches[$form_state['values']['develop_branch']];
      $workflow_args['develop']['id'] = $form_state['values']['develop_branch'];
      break;
    // Dev -> stage -> prod
    case 1:
      // Set the args for the prod branch
      $workflow_args['prod']['string'] = $branches[$form_state['values']['prod_branch']];
      $workflow_args['prod']['id'] = $form_state['values']['prod_branch'];
      // Set the args for the stage branch
      $workflow_args['stage']['string'] = $branches[$form_state['values']['stage_branch']];
      $workflow_args['stage']['id'] = $form_state['values']['stage_branch'];
      // Set the args for the dev branch
      $workflow_args['dev']['string'] = $branches[$form_state['values']['develop_branch']];
      $workflow_args['dev']['id'] = $form_state['values']['develop_branch'];
      break;
    // Dev -> prod
    case 2:
      // Set the args for the prod branch
      $workflow_args['prod']['string'] = $branches[$form_state['values']['prod_branch']];
      $workflow_args['prod']['id'] = $form_state['values']['prod_branch'];
      // Set the args for the dev branch
      $workflow_args['dev']['string'] = $branches[$form_state['values']['develop_branch']];
      $workflow_args['dev']['id'] = $form_state['values']['develop_branch'];
      break;
  }
  // JSON encode the workflow_args
  $workflow_args['hotfix'] = isset($form_state['values']['hotfix']);
  $workflow_args = drupal_json_encode($workflow_args);

  // Check if the gitinfo already exists.
  $site_id = arg(3);
  if (gitinfo_exists($site_id)) {
    // Update the gitinfo entity.
    $gitinfo_id = gitinfo_getid($site_id);
    if ($gitinfo_id != FALSE) {
      // Load and update each of the values of the gitinfo entity.
      $gitinfo = gitinfo_load($gitinfo_id);
      $gitinfo->name = parse_repo_name($gitapi_clone_url);
      $gitinfo->service = $gitapi_service;
      $gitinfo->clone_url = $gitapi_clone_url;
      $gitinfo->branches = $branches;
      $gitinfo->is_make = $form_state['values']['gitapi_makefile'];
      $gitinfo->make_location = $form_state['values']['gitapi_makefile_location'];
      $gitinfo->workflow = $workflow;
      $gitinfo->workflow_args = $workflow_args;
      $gitinfo->save();
    }
  } else {
    // Create the gitinfo entity.
    $gitinfo = entity_create('gitinfo', array(
      'site' => samurai_get_site_id($form_state['multistep_values']['client_site_form_step_1']['name']),
      'name' => parse_repo_name($gitapi_clone_url),
      'service' => $gitapi_service,
      'clone_url' => $gitapi_clone_url,
      'branches' => $branches,
      'is_make' => $form_state['values']['gitapi_makefile'],
      'make_location' => $form_state['values']['gitapi_makefile_location'],
      'workflow' => $workflow,
      'workflow_args' => $workflow_args,
    ));
    $gitinfo_wrapper = entity_metadata_wrapper('gitinfo', $gitinfo);
    $gitinfo_wrapper->save();
  }
}

/**
 * Client site form step 5
 *
 * Submit handler @see client_site_form_submit_details_step_5
 */
function client_site_form_step_5($form, $form_state) {

  // Retrieve information from the previous steps
  $chosen_workflow = $form_state['multistep_values']['client_site_form_step_3']['gitapi_workflow_model'];
  $site_id = samurai_get_site_id($form_state['multistep_values']['client_site_form_step_1']['name']);
  // Load the variable with server info
  $projects = security_samurai_deploy_retrieve_projects();
  $selected_project = $projects[$form_state['multistep_values']['client_site_form_step_4']['deploy_project']];
  $deploy_servers = security_samurai_deploy_retrieve_project_servers($selected_project);
  // Load the deployinfo entity if exists
  $deployinfo = NULL;
  if (deployinfo_exists($site_id)) {
    $deployinfo_id = deployinfo_getid($site_id);
    if (!is_null($deployinfo_id)) {
      $deployinfo = deployinfo_load($deployinfo_id);
    }
  }

  // Fieldset for the form elements
  $form['workflow_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customize the workflow'),
    '#fieldset_icon' => 'edit',
  );
  switch ($chosen_workflow) {
    case 0:
      if (!is_null($deployinfo)) {
        $args = drupal_json_decode($deployinfo->args);
        $master_branch_default = security_samurai_deploy_get_server_key($args['master'], $selected_project);
        $develop_branch_default = security_samurai_deploy_get_server_key($args['develop'], $selected_project);
      } elseif (isset($form_state['values']['master_branch'])) {
        $master_branch_default = $form_state['values']['master_branch'];
        $develop_branch_default = $form_state['values']['develop_branch'];
      } else {
        $master_branch_default = NULL;
        $develop_branch_default = NULL;
      }
      $form['workflow_fieldset']['master_branch'] = array(
        '#type' => 'select',
        '#title' => t('Server for the \'master\' branch'),
        '#description' => t('The server to deploy to when a change to the \'master\' branch is made'),
        '#required' => TRUE,
        '#options' => $deploy_servers,
        '#default_value' => $master_branch_default,
      );
      $form['workflow_fieldset']['develop_branch'] = array(
        '#type' => 'select',
        '#title' => t('Server for the \'develop\' branch'),
        '#description' => t('The server to deploy to when a change to the \'develop\' branch is made'),
        '#required' => TRUE,
        '#options' => $deploy_servers,
        '#default_value' => $develop_branch_default,
      );
      break;
    case 1:
      $form['workflow_fieldset']['dev_branch'] = array(
        '#type' => 'select',
        '#title' => t('Server for the \'dev\' branch'),
        '#description' => t('The server to deploy to when a change to the \'dev\' branch is made'),
        '#required' => TRUE,
        '#options' => $deploy_servers,
      );
      $form['workflow_fieldset']['stage_branch'] = array(
        '#type' => 'select',
        '#title' => t('Server for the \'stage\' branch'),
        '#description' => t('The server to deploy to when a change to the \'stage\' branch is made'),
        '#required' => TRUE,
        '#options' => $deploy_servers,
      );
      $form['workflow_fieldset']['prod_branch'] = array(
        '#type' => 'select',
        '#title' => t('Server for the \'prod\' branch'),
        '#description' => t('The server to deploy to when a change to the \'prod\' branch is made'),
        '#required' => TRUE,
        '#options' => $deploy_servers,
      );
      break;
    case 2:
      $form['workflow_fieldset']['dev_branch'] = array(
        '#type' => 'select',
        '#title' => t('Server for the \'dev\' branch'),
        '#description' => t('The server to deploy to when a change to the \'dev\' branch is made'),
        '#required' => TRUE,
        '#options' => $deploy_servers,
      );
      $form['workflow_fieldset']['prod_branch'] = array(
        '#type' => 'select',
        '#title' => t('Server for the \'prod\' branch'),
        '#description' => t('The server to deploy to when a change to the \'prod\' branch is made'),
        '#required' => TRUE,
        '#options' => $deploy_servers,
      );
      break;
  }
  $form['chosen_workflow'] = array(
    '#type' => 'hidden',
    '#value' => $chosen_workflow,
  );
  // Form back button
  $form['form_back'] = array(
    '#type' => 'submit',
    '#value' => 'Back',
  );
  // Form submit button
  $form['form_next'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * Form submit handler
 * @see client_site_form_submit_details_step_5
 */
function client_site_form_submit_details_step_5($form, $form_state) {

  // Retrieve values from previous steps
  $projects = security_samurai_deploy_retrieve_projects();
  $selected_project = $projects[$form_state['multistep_values']['client_site_form_step_4']['deploy_project']];
  $servers = security_samurai_deploy_retrieve_project_servers($selected_project);
  // Create the args
  $args = array();
  switch ($form_state['values']['chosen_workflow']) {
    // Git flow
    case 0:
      $args['project'] = $selected_project;
      $args['master'] = $servers[$form_state['values']['master_branch']];
      $args['develop'] = $servers[$form_state['values']['develop_branch']];
      break;
    // Dev -> stage -> prod
    case 1:
      $args['project'] = $selected_project;
      $args['prod'] = $servers[$form_state['values']['prod_branch']];
      $args['stage'] = $servers[$form_state['values']['stage_branch']];
      $args['dev'] = $servers[$form_state['values']['dev_branch']];
      break;
    // Dev -> prod
    case 2:
      $args['project'] = $selected_project;
      $args['prod'] = $servers[$form_state['values']['prod_branch']];
      $args['dev'] = $servers[$form_state['values']['dev_branch']];
      break;
  }
  // Encode the args
  $args = drupal_json_encode($args);
  // Submit the deployment information
  $site_id = arg(3);
  if (deployinfo_exists($site_id)) {
    // Update the existing deployinfo entity
    $deployinfo_id = deployinfo_getid($site_id);
    if (!is_null($deployinfo_id))  {
      $deployinfo = deployinfo_load($deployinfo_id);
      $deployinfo->deploy_service_provider = $form_state['multistep_values']['client_site_form_step_3']['deploy_service'];
      $deployinfo->args = $args;
      $deployinfo->save();
    }
  } else {
    // Create a new deployinfo entity
    $deployinfo = entity_create('deployinfo', array(
      'name' => 'deployinfo',
      'client_site_id' => samurai_get_site_id($form_state['multistep_values']['client_site_form_step_1']['name']),
      'gitinfo_id' => gitinfo_getid($site_id),
      'deploy_service_provider' => $form_state['multistep_values']['client_site_form_step_3']['deploy_service'],
      'args' => $args
    ));
    $deployinfo_wrapper = entity_metadata_wrapper('deployinfo', $deployinfo);
    $deployinfo_wrapper->save();
  }
}
