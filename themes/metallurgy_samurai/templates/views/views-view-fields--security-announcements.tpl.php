<?php

/**
 * @file
 * View template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>
<?php foreach ($fields as $id => $field): ?>
  <?php if (!empty($field->separator)): ?>
    <?php print $field->separator; ?>
  <?php endif; ?>

  <?php print $field->wrapper_prefix; ?>
    <?php print $field->label_html; ?>
    <?php if ($field->handler->field_alias == 'securitysamurai_sa_risk_level'): ?>
        <span class="hide-on-med-only hide-on-small-only risk-<?php print samurai_parse_risk($field->raw); ?>">
          <?php print $field->content; ?>
        </span>
        <span class="hide-on-large-only risk-<?php print samurai_parse_risk($field->raw); ?>">
          <?php $position = strpos($field->content, ')') + 1; ?>
          <?php print substr($field->content, 0, $position); ?>
        </span>
    <?php else: ?>
        <?php print $field->content ?>
    <?php endif; ?>
  <?php print $field->wrapper_suffix; ?>
<?php endforeach; ?>
