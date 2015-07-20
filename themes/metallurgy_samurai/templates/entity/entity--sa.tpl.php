<div class="content-title span-12 float-left">
  <div id="content-header">
    <span class="risk-<?php print samurai_parse_risk($element->risk_level); ?>">
      <h1 id="title"><?php print render($element->sa_id); ?></h1>
    </span>
    <h1 id="title-type">Security announcement</h1>
  </div>
</div>
<div class="span-8 float-left">
  <div id="content-main-wrapper">
    <div id="content-main">
      <h4 class="publish-info card">Published: <?php print date('d-m-Y H:i:s', $element->date); ?></h4>
      <div class="card">
        <h2 class="title">Affected project</h2>
        <p class="content"><?php print render($element->project_name);?> <?php $element_version = implode(',', drupal_json_decode($element->version)); print render($element_version); ?></p>
      </div>
      <div class="card">
        <h2 class="title">Risk level</h2>
        <p class="content"><?php print render($element->risk_level); ?></p>
      </div>
      <div class="card">
        <h2 class="title">Vulnerability</h2>
        <p class="content"><?php print render($element->vulnerability); ?></p>
      </div>
      <div class="card">
        <h2 class="title">Solution</h2>
        <p class="content"><?php print render($element->solution); ?></p>
      </div>
    </div>
  </div>
</div>
<div id="column-right-wrapper" class="span-4 float-left padding-left">
  <div id="column-right">
    <div class="card" style="margin-top: 0px;">
      <h2 class="title">Sites affected by this</h2>
      <?php $result = samurai_get_affected_sites($element->sa_id, $element->project_name, $element->version); ?>
      <?php if (!is_null($result)): ?>
        <?php print $result; ?>
      <?php else: ?>
        <p class="content">Doesn't look like anything is here</p>
      <?php endif; ?>
    </div>
  </div>
</div>
