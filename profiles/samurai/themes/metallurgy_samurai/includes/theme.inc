<?php
/**
 * @file
 * theme.inc
 */


/**
 * Implements theme_status_message()
 */
function metallurgy_samurai_status_messages($variables) {

  $display = $variables ['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );
  foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= "<script type='text/javascript'>";
    if (count($messages) > 1) {
      foreach ($messages as $message) {
        $output .= 'Materialize.toast("' . str_replace('"', "'", $message) . '", 5000);';
      }
    }
    else {
      $output .= 'Materialize.toast("' . str_replace('"', "'", $messages[0]) . '", 5000);';
    }
    $output .= "</script>\n";
  }
  return $output;
}

/**
 * Implements theme_fieldset
 */
function metallurgy_samurai_fieldset($variables) {

  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));

  $element['#attributes']['class'][] = 'collapsible';
  if ($element['#collapsed'] == FALSE) {
    $element['#attributes']['class'][] = 'active';
  }
  $element['#attributes']['data-collapsible'][] = 'expandable';

  $output = '<ul' . drupal_attributes($element ['#attributes']) . '>';
  if ($element['#collapsed'] == TRUE) {
    $output .= '<li>';
  } else {
    $output .= '<li class="active">';
  }
  if (!empty($element ['#title'])) {
    if (!empty($element['#fieldset_icon'])) {
      // Always wrap fieldset legends in a SPAN for CSS positioning.
      $output .= '<div class="collapsible-header"><i class="material-icons">' . $element['#fieldset_icon'] . '</i><span class="fieldset-legend">' . $element ['#title'] . '</span></div>';
    } else {
      // Always wrap fieldset legends in a SPAN for CSS positioning.
      $output .= '<div class="collapsible-header"><span class="fieldset-legend">' . $element ['#title'] . '</span></div>';
    }
  }
  if ($element['#collapsed'] == FALSE) {
    $output .= '<div class="collapsible-body" style="padding: 1em;display: block;">';
  } else {
    $output .= '<div class="collapsible-body" style="padding: 1em;">';
  }
  if (!empty($element ['#description'])) {
    $output .= '<div class="fieldset-description">' . $element ['#description'] . '</div>';
  }
  $output .= $element ['#children'];
  if (isset($element ['#value'])) {
    $output .= $element ['#value'];
  }
  $output .= '</div>';
  $output .= '</li>';
  $output .= "</ul>\n";
  return $output;
}

/**
 * Implements theme_button
 */
function metallurgy_samurai_button($variables) {

  $element = $variables ['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element ['#button_type'];
  $element['#attributes']['class'][] = 'waves-effect';
  $element['#attributes']['class'][] = 'waves-light';
  $element['#attributes']['class'][] = 'btn';
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  if (isset($element['#attributes']['type'])) {
    $return = '<button';
    $return .= drupal_attributes($element['#attributes']);
    $return .= '>';
    $return .= $element['#value'];
    $return .= '</button>';
    return $return;
  }

  return '<input' . drupal_attributes($element ['#attributes']) . ' />';
}

/**
 * Implements hook_preprocess_textarea()
 */
function metallurgy_samurai_preprocess_textarea(&$variables) {

  // Add a class to the textarea element
  $variables['element']['#attributes']['class'][] = 'materialize-textarea';
}

/**
 * Returns HTML for a progress bar.
 *
 * Note that the core Batch API uses this only for non-JavaScript batch jobs.
 *
 * @param $variables
 *   An associative array containing:
 *   - percent: The percentage of the progress.
 *   - message: A string containing information to be displayed.
 */
function metallurgy_samurai_progress_bar($variables) {
  $output = '<div class="progress">';
  $output .= '<div class="indeterminate"></div>';
  $output .= '</div>';
  
  return $output;
}

