<div id="left-content">
  <div id="content-header-wrapper">
    <div id="content-header">
      <h1 id="title"><?php print $element->name; ?></h1>
      <h1 id="title-type">Client site</h1>
    </div>
  </div>
  <div id="content-main-wrapper">
    <div id="content-main">
      <h4 class="publish-info">Created: <?php print date('d-m-Y H:i:s', $element->created_at); ?></h4>
      <div class="card">
        <h2 class="title">Site information</h2>
        <?php if ($element->update_next_check > 0): ?>
          <p class="content">The site will be scanned for new projects automatically at <?php print date('d-m-Y H:i:s', $element->update_next_check); ?>.</p>
        <?php else: ?>
          <p class="content">The site will be scanned on the next cron run (less than 5 minutes).</p>
        <?php endif; ?>
      </div>
      <div class="card">
        <h2 class="title">testing</h2>
        <p class="content">Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.</p>
      </div>
    </div>
  </div>
</div>
<div id="right-content">
  <div id="column-right-wrapper">
    <div id="column-right">
      <!-- Print the block manually -->
      <div class="card">
        <h2 class="title">Scan the site now</h2>
        <p class="content">button here</p>
      </div>
    </div>
  </div>
</div>