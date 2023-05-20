<?php /* Smarty version 2.6.16, created on 2019-02-04 21:31:54
         compiled from controller_head.tpl */ ?>



<div id="reset_controller_prompt" title="Перезагрузка" class='hdn'>
  <p>Вы уверены, что хотите перезагрузить контроллер?</p>
</div>

<div id="reset_in_process" title="Перезагрузка" class='hdn'>
  <p>Подождите, пока контроллер перезагрузится...</p>
</div>

<ul class='navbar'>
<li><a href="/" id='back_link'>На главную</a>
<li <?php if ($this->_tpl_vars['room'] == 'status'): ?>class='active'<?php endif; ?>><a href="/controller.php?id=<?php echo $this->_tpl_vars['selected_controller']['controller_id']; ?>
" id='view_link'>Показания</a></li>
<li <?php if ($this->_tpl_vars['room'] == 'settings'): ?>class='active'<?php endif; ?>><a href="/controller_settings.php?id=<?php echo $this->_tpl_vars['selected_controller']['controller_id']; ?>
" id='settings_link'>Настройки</a></li>
<li <?php if ($this->_tpl_vars['room'] == 'charts'): ?>class='active'<?php endif; ?>><a href="/controller_charts.php?id=<?php echo $this->_tpl_vars['selected_controller']['controller_id']; ?>
" id='charts_link'>Графики</a></li>
<li style="float:right;"><a href="javascript:resetController();" id='reset_controller_link' style='color:coral;'>Перезагрузить</a></li>
</ul>

<h1 style="margin-bottom:0px;">Контроллер "<?php echo $this->_tpl_vars['selected_controller']['controller_name']; ?>
"<?php echo $this->_tpl_vars['additional_text']; ?>
 <div class='right_float' id='controller_date_time'></div></h1>


<div class='info-success hdn' id='controller_stats'>
  <div class='controller_stats' style="padding-bottom:5px;">
    
    <div class='freeram hdn' id='freeram_box'>
      Память: <span class='bold' id='controller_freeram'></span> байт
    </div>
    
    <div class='uptime hdn' id='uptime_box'>
      Время работы: <span class='bold' id='controller_uptime'></span>
    </div>   
    
  </div>
</div>

<div style="margin-bottom:20px;"></div>



<script type='text/javascript'>
$(document).ready(function(){
/*
  $( "#back_link" ).button({
      icons: {
        primary: "ui-icon-seek-prev"
      }
    });
 
  $( "#view_link" ).button({
      icons: {
        primary: "ui-icon-tag"
      }
    });
    
  $( "#settings_link" ).button({
      icons: {
        primary: "ui-icon-gear"
      }
    });
    
  $( "#charts_link" ).button({
      icons: {
        primary: "ui-icon-image"
      }
    });    
 
  $( "#reset_controller_link" ).button({
      icons: {
        primary: "ui-icon-refresh"
      }
    }).css('background','#ff794d').hide();            
  */  
    if(typeof(controller) != 'undefined')
    {
      if(controller.IsOnline())
      {
        upTime();
        freeRam();
      }
      window.setInterval(freeRam,FREERAM_CHECK_INTERVAL);
      
      window.setInterval(upTime,UPTIME_CHECK_INTERVAL);
      
      waitControllerTimer = window.setInterval(waitControllerTime,1000);
      
    }
              

});  
</script>