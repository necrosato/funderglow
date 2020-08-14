String setHtml = "<!DOCTYPE html>\
<html>\
    <head>\
        <title>Funderglow</title>\
    </head>\
    <body>\
      <form action=\"/set\">\
        <label for=\"red\">Red</label>\
        <input type=\"number\" id=\"red\" name=\"red\"><br>\
        <label for=\"green\">Green:</label>\
        <input type=\"number\" id=\"green\" name=\"green\"><br>\
        <label for=\"blue\">Blue:</label>\
        <input type=\"number\" id=\"blue\" name=\"blue\"><br>\
        <input type=\"submit\" value=\"Set Colors\">\
      </form>\
      <br>\
      <button onclick=\"document.location='wave'\">Waves</button>\
      <br>\
      <button onclick=\"document.location='test'\">Test</button>\
    </body>\
</html>";
