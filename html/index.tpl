    <!DOCTYPE html>
    <html>
          <head>
                <meta name="viewport" content="width=device-width, initial-scale=1">
                <style type="text/css">
                      html, body, iframe { margin: 0; padding: 0; height: 100%; }
                      iframe { display: block; width: 100%; border: none; }
                </style>
                <title>BEEBOT SETUP</title>
          </head>
          <body>
                <h1>Welcome to BEEBOT wifi setup!</h1>
                <form action="connect.cgi" method="post">
                      ssid: <input type="text" name="ssid"><br>
                      password: <input type="password" name="pass"><br>
                      <input type="submit" value="Submit">
                </form>
                <form action="resetnvs.cgi" method="post">
                      <input type="submit" value="reset NVS">
                </form>
          </body>
    </html>
