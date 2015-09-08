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
  if (!isset($form_state['step'])) $form_state['step'] = 'client_site_form_step_1';

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
  }
}

/**
 * Return the previous step function name
 */
function client_site_form_move_to_previous($form, $form_state) {

  switch ($form_state['step']) {

    case 'client_site_form_step_3':
      return 'client_site_form_step_2';
      break;
    case 'client_site_form_step_2':
      return 'client_site_form_step_1';
      break;
  }
}

/**
 * Client form step 1
 */
function client_site_form_step_1($form, $form_state, $site = NULL) {

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
  if (isset($site->name)) {
    $name_default = $site->name;
  } else if (isset($form_state['values']['name'])) {
    $name_default = $form_state['values']['name'];
  } else {
    $name_default = '';
  }
  $form['client_fieldset']['name'] = array(
    '#title' => t('Site name'),
    '#type' => 'textfield',
    '#default_value' => $name_default,
    '#description' => t('The name of the site.'),
    '#required' => TRUE,
  );

  // The encryption key.
  if (isset($site->encryption_key)) {
    $encryption_default = $site->encryption_key;
  } else if (isset($form_state['values']['encryption_key'])) {
    $encryption_default = $form_state['values']['encryption_key'];
  } else {
    $encryption_default = '';
  }
  $form['client_fieldset']['encryption_key'] = array(
    '#title' => t('Encryption key'),
    '#type' => 'textfield',
    '#default_value' => $encryption_default,
    '#description' => t('The key provided by the site.'),
    '#required' => TRUE,
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

  $form['client_fieldset']['drush_alias'] = array(
    '#title' => t('Drush alias'),
    '#description' => t('The drush alias used by the site'),
    '#type' => 'select',
    '#options' => variable_get('security_samurai_aliases', array(1 => 'No aliases available')),
    '#default_value' => isset($site->site_alias) ? $site->site_alias : 0,
    '#required' => TRUE,
  );

  // Default form submit actions.
  $form['form_next'] = array(
    '#type' => 'submit',
    '#value' => 'Next',
  );

  // Return the form components.
  return $form;
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
    case 'client_site_form_step_3':
      return client_site_form_step_3_validate($form, $form_state);
      break;
  }
}

/**
 * Form validation handler for the site details step of the client_site_form
 */
function client_site_form_step_1_validate($form, $form_state) {

  // The encryption key must be 32 or more characters long.
  if (strlen($form_state['values']['encryption_key']) < 32) {
    // Error handling.
    return form_set_error('encryption_key', t('Please enter a valid encryption key.'));
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
}

/**
 * The client_site_form form submit handler
 */
function client_site_form_submit($form, &$form_state) {

  switch ($form_state['step']) {

    case 'client_site_form_step_3':
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

  if (isset($form_state['complete'])) drupal_goto('/client-site/config');

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

  // -- Save the entity.
  // -- Print notification messages.
  // -- Log to watchdoge.
  // -- Redirect to site/add.
  $site_id = arg(3);
  if (!samurai_site_exists($form_state['values']['name']) && is_null($site_id)) {

    // Entity doesn't exist.
    $site = entity_create('client_site', array(
      'name' => $form_state['values']['name'],
      'encryption_key' => $form_state['values']['encryption_key'],
      'site_url' => $form_state['values']['site_url'],
      'update_next_check' => 0,
      'site_alias_id' => $form_state['values']['drush_alias'],
      // 'site_alias_name' =>
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
    // $site->site_alias_name = /** @todo */
    $site->update_next_check = 0;
    $site->save();
  }
  drupal_set_message(t('@name has been saved.', array('@name' => $site->name)));
  watchdog('Security samurai', t('@name has been saved.', array('@name' => $site->name)));
  $form_state['redirect'] = 'site/config';
}
