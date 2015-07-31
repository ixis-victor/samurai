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
    $output .= "<div class=\"messages card $type\">\n";
    if (!empty($status_heading [$type])) {
      $output .= '<h2 class="element-invisible">' . $status_heading [$type] . "</h2>\n";
    }
    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= reset($messages);
    }
    $output .= "</div>\n";
  }
  return $output;
}

/**
 * Implements theme_fieldset
 */
function metallurgy_samurai_fieldset($variables) {
  $element = $variables ['element'];
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
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<div class="collapsible-header"><span class="fieldset-legend">' . $element ['#title'] . '</span></div>';
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