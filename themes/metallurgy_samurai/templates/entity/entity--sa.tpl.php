<div class="span-8 float-left">
  <div id="content-main-wrapper">
    <div id="content-main">
      <h4 class="publish-info card">Published: <?php print date('d-m-Y H:i:s', $element->date); ?></h4>
      <div class="card popup">
        <div class="card-content">
          <span class="card-title">Affected project</span>
          <p><?php print render($element->project_name);?> <?php $element_version = implode(',', drupal_json_decode($element->version)); print render($element_version); ?></p>
        </div>
      </div>
      <div class="card popup">
        <div class="card-content">
          <span class="card-title">Risk level</span>
          <p><?php print render($element->risk_level); ?></p>
        </div>
      </div>
      <div class="card popup">
        <div class="card-content">
          <span class="card-title">Vulnerability</span>
          <p><?php print render($element->vulnerability); ?></p>
        </div>
      </div>
      <div class="card popup">
        <div class="card-content">
          <span class="card-title">Solution</span>
          <p><?php print render($element->solution); ?></p>
        </div>
      </div>
    </div>
  </div>
</div>
<div id="column-right-wrapper" class="span-4 float-left padding-left">
  <div id="column-right">
    <div class="card popup" style="margin-top: 0px;">
      <div class="card-content">
        <span class="card-title">Sites affected by this</span>
        <?php $result = samurai_get_affected_sites($element->sa_id, $element->project_name, $element->version); ?>
        <?php if (!is_null($result)): ?>
          <?php print $result; ?>
        <?php else: ?>
          <p>No sites affected by this vulnerability</p>
        <?php endif; ?>
      </div>
    </div>
  </div>
</div>
