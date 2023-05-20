{* Smarty *}



<div id="reset_controller_prompt" title="Перезагрузка" class='hdn'>
  <p>Вы уверены, что хотите перезагрузить контроллер?</p>
</div>

<div id="reset_in_process" title="Перезагрузка" class='hdn'>
  <p>Подождите, пока контроллер перезагрузится...</p>
</div>

<ul class='navbar'>
<li><a href="/" id='back_link'>На главную</a>
<li {if $room=='status'}class='active'{/if}><a href="/controller.php?id={$selected_controller.controller_id}" id='view_link'>Показания</a></li>
<li {if $room=='settings'}class='active'{/if}><a href="/controller_settings.php?id={$selected_controller.controller_id}" id='settings_link'>Настройки</a></li>
<li {if $room=='charts'}class='active'{/if}><a href="/controller_charts.php?id={$selected_controller.controller_id}" id='charts_link'>Графики</a></li>
<li style="float:right;"><a href="javascript:resetController();" id='reset_controller_link' style='color:coral;'>Перезагрузить</a></li>
</ul>

<h1 style="margin-bottom:0px;">Контроллер "{$selected_controller.controller_name}"{$additional_text} <div class='right_float' id='controller_date_time'></div></h1>


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
$(document).ready(function(){ldelim}
/*
  $( "#back_link" ).button({ldelim}
      icons: {ldelim}
        primary: "ui-icon-seek-prev"
      {rdelim}
    {rdelim});
 
  $( "#view_link" ).button({ldelim}
      icons: {ldelim}
        primary: "ui-icon-tag"
      {rdelim}
    {rdelim});
    
  $( "#settings_link" ).button({ldelim}
      icons: {ldelim}
        primary: "ui-icon-gear"
      {rdelim}
    {rdelim});
    
  $( "#charts_link" ).button({ldelim}
      icons: {ldelim}
        primary: "ui-icon-image"
      {rdelim}
    {rdelim});    
 
  $( "#reset_controller_link" ).button({ldelim}
      icons: {ldelim}
        primary: "ui-icon-refresh"
      {rdelim}
    {rdelim}).css('background','#ff794d').hide();            
  */  
    if(typeof(controller) != 'undefined')
    {ldelim}
      if(controller.IsOnline())
      {ldelim}
        upTime();
        freeRam();
      {rdelim}
      window.setInterval(freeRam,FREERAM_CHECK_INTERVAL);
      
      window.setInterval(upTime,UPTIME_CHECK_INTERVAL);
      
      waitControllerTimer = window.setInterval(waitControllerTime,1000);
      
    {rdelim}
              

{rdelim});  
</script>