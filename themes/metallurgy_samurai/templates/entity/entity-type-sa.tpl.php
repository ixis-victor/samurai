<div id="left-content">
  <div id="content-header-wrapper">
    <div id="content-header">
      <span class="risk-<?php print samurai_parse_risk($element->risk_level); ?>">
        <h1 id="title"><?php print render($element->sa_id); ?></h1>
      </span>
      <h1 id="title-type">Security announcement</h1>
    </div>
  </div>
  <div id="content-main-wrapper">
    <div id="content-main" class="span_8">
      <h4 class="publish-info">Published: <?php print date('d-m-Y H:i:s', $element->date); ?></h4>
      <div class="card">
        <h2 class="title">Affected project</h2>
        <p class="content"><?php print render($element->project_name);?> <?php print render(implode(',', drupal_json_decode($element->version))); ?></p>
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
<div id="right-content" class="span_3">
  <div id="column-right-wrapper">
    <div id="column-right">
      <div class="card">
        <h2 class="title">Sites affected by this</h2>
        <?php $result = samurai_get_affected_clients($element->sa_id, $element->project_name, $element->version); ?>
        <?php if (!is_null($result)): ?>
          <?php print $result; ?>
        <?php else: ?>
          <p class="content">Doesn't look like anything is here</p>
        <?php endif; ?>
      </div>
    </div>
  </div>
</div>
