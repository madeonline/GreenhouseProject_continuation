{* Smarty *}

<script type="text/javascript" src="js/edit_controller.js"></script>

<div id='no_controllers_hint' class='hdn'>
  <div class="controller_list_hint ui-state-error ui-corner-all error">
    <img src="/images/logo48.png" class="logo">Нет ни одного контроллера. Для добавления контроллера нажмите кнопку ниже.
  </div>
</div>

<div id="controller_dialog" title="Данные контроллера" class='hdn'>
  <form>
  {*
  ID контроллера:<br/>
  <input type='text' id='edit_controller_id' maxlength='5' value='' style='width:100%;'/><br/>
 *}
  Имя контроллера:<br/>
  <input type='text' id='controller_name' maxlength='50' value='' style='width:100%;'/><br/>
  Адрес контроллера:<br/>
  <input type='text' id='controller_address' maxlength='100' value='' style='width:100%;'/><br/>
  </form>
</div>

<div id='message_box_dialog' class='hdn'>
  <div id='message_box_text'></div>
</div>

<div id='controller_online' class='hdn'><span class='auto_mode'>да</span></div>
<div id='controller_offline' class='hdn'><span class='manual_mode'>нет</span></div>

<div id="controller_in_process_dialog" title="Идёт обработка данных..." class='hdn'>
  <p>Пожалуйста, подождите, пока данные обрабатываются...</p>
</div>

<script type='text/javascript'>
var controllers = new Array();

{literal}
function setupController(c) {
    c.OnStatus = function(controller) {
      var stat = controller.IsOnline() ? $('#controller_online').html() :  $('#controller_offline').html();
      $("#controller_status" + controller.getId()).html(stat);
    };
} 

function addController(id,name,address) {
  var newC = new Controller(id,name,address);
  controllers.push(newC);
  return newC;
}
  
 $(document).ready(function(){
 
  $( "#add_new_controller_btn" ).button({
      icons: {
        primary: "ui-icon-plus"
      }
    });
    
  
 });
 
 $(document).ready(function(){
  setupControllerButtons();
  addControllerListHint();
 });
  
{/literal}

{foreach key=k item=controller from=$controllers}
addController({$controller.controller_id},'{$controller.controller_name}','{$controller.controller_address}');
{/foreach}

{literal}
  for(var i=0;i<controllers.length;i++)
  {
    var c = controllers[i];
    setupController(c);

  }
{/literal}

</script>

<h1><span class='innerh'>Список контроллеров</span></h1>
<div id='controllers_list'>
{foreach key=k item=controller from=$controllers}
{include file='controller_info.tpl'}
{/foreach}
</div>

<div style='clear:both;padding-top:20px;'>
  <a href='#' onclick="addNewController();return false;" id='add_new_controller_btn'>Добавить новый контроллер</a>
</div>
