$(document).ready(

    function(){
      console.log(" documentation hello ");
      $('#top_list').columnize({ columns: 3 });
      //$('.documentation_index_group').columnize({ width:300, lastNeverTallest: true  });



      $("pre").each(function(){
        children = $(this).children("code");
        if(children.length>0){
            $(this).text($(children[0]).text());
            this.className="brush: " + children[0].className;
        }    
      });
      
      SyntaxHighlighter.defaults['toolbar'] = false;
      SyntaxHighlighter.all()
      
      $(".documentation_detail").hide();
      var selected = location.hash.substring(6);
      if(selected.length){  
        $(".documentation_detail."+selected).show();
        $(".functionslist").find('a.'+selected).addClass('selected');
      }else{
        $(".class_documentation").show();
        $("#docstitle h1").addClass('selected');
      }
      $(".functionslist").find('a').click(function(){
        $(".documentation_detail").hide();
        $(".documentation_detail."+$(this).attr('class')).show();
        $(".functionslist").find('a').removeClass('selected');
        $(this).addClass('selected');
        $("#docstitle h1").removeClass('selected');
        var anchor = $(this)
        $('html, body').animate({scrollTop:0}, 200);
      });
     $("#docstitle h1").click(function(){
        $(".documentation_detail").hide();
        $(".documentation_detail.class_documentation").show();
        $(".functionslist").find('a').removeClass('selected');
        $(this).addClass('selected');
      });
    });

