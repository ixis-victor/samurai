<div class="content-title span-12 float-left">
  <div id="content-header">
    <h1 id="title"><?php print $element->name; ?></h1>
    <h1 id="title-type">Site</h1>
  </div>
</div>
<div class="span-8 float-left">
  <div id="content-main-wrapper">
    <div id="content-main">
      <h4 class="publish-info card">Created: <?php print date('d-m-Y H:i:s', $element->created_at); ?></h4>
      <div class="card">
        <div class="card-content">
        <span class="card-title">Site information</span>
          <?php if ($element->update_next_check > 0): ?>
            <p>The site will be scanned for new projects automatically at <?php print date('d-m-Y H:i:s', $element->update_next_check); ?>.</p>
          <?php else: ?>
            <p>The site will be scanned on the next cron run (less than 5 minutes).</p>
          <?php endif; ?>
        </div>
      </div>
      <!-- print a theme grid right here. -->
      <?php $grid_data = samurai_render_update_data($element->site_update_data); ?>
      <?php if (!is_null($grid_data)): ?>
        <table class="client-projects card">
          <thead>
            <tr>
              <th>Name</th>
              <th>Type</th>
              <th>Installed version</th>
              <th>Latest version</th>
              <th>Latest secure version</th>
            </tr>
          </thead>
        <?php foreach($grid_data as $data): ?>
          <tr>
            <td><?php print $data['name']; ?></td>
            <td><?php print $data['type']; ?></td>
            <td <?php 
              if (isset($data['status'])) {
                print 'class="' . $data['status'] . '"';
              }
            ?>><?php print $data['installed_version']; ?></td>
            <td><?php print $data['latest_version']; ?></td>
            <td><?php print $data['secure_version']; ?></td>
          </tr>
        <?php endforeach; ?>
        </table>
      </div>
      <?php else: ?>
        <div class="card">
          <div class="card-content">
            <span class="card-title">No projects found</span>
            <p class="content">Doesn't look like any projects for this site exist. Try scanning the site.</p>
          </div>
        </div>
      <?php endif; ?>
    </div>
  </div>
</div>
<div id="column-right-wrapper" class="span-4 float-left padding-left">
  <div id="column-right">
    <!-- Print the block manually -->
    <div class="card" style="margin-top: 0px;">
      <div class="card-content">
        <span class="card-title">Scan the site now</span>
        <p class="content">Coming soon...</p>
      </div>
    </div>
  </div>
</div>
