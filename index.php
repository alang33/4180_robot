<?php
    include('session.php');
    
    function console_log($output, $with_script_tags = true) {
        $js_code = 'console.log('.json_encode($output, JSON_HEX_TAG).');';
        if ($with_script_tags) {
            $js_code = '<script>'.$js_code.'</script>';
        }
        echo $js_code;
    }
    
    if($_SERVER["REQUEST_METHOD"] == "POST"){
        $song = $_POST["song"];
        $artist = $_POST["artist"];
        $hours = $_POST["hours"];
        $minutes = $_POST["minutes"];
        $txt = "$song-$artist\n";
        $txt_2 = "$hours$minutes\n";
        if(isset($_POST["add"])){
            $file = fopen("/var/www/html/song.txt", "r");
            $file_2 = fopen("/var/www/html/alarm.txt", "r");
            $status = fread($file,4);
            $status_2 = fread($file_2,4);       
            fclose($file);
            fclose($file_2);
            #if ($status == "NULL"){
              $file = fopen("/var/www/html/song.txt", "w");
              fwrite($file, $txt);
              fclose($file);
            #}
            #if ($status_2 == "NULL"){
               $file_2 = fopen("/var/www/html/alarm.txt", "w");
               fwrite($file_2, $txt_2);
               fclose($file_2);
            #}
        exec("mode /dev/ttyACM0 BAUD=9600 PARITY=N data=8 stop=1, xon=off");
        $send=fopen("/var/www/html/pi_clock/alarm_time.py", "w+");
             fwrite($send, "sdfsdf");
             fclose($send);
        
        $success = shell_exec("python /var/www/html/pi_clock/Play_song.py > /dev/null &");
        $success = 1;
        
        
        
        }
    }
?>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Boombox</title>
    <script src = "http://code.jquery.com/jquery-2.1.0.js"></script>
    <link href='https://fonts.googleapis.com/css?family=Strait' rel='stylesheet'>
    <style type="text/css">
	
    body { 
  	margin: 0;
      color:#f0f0f0;
      font-family: 'Strait';font-size: 2rem;
      background-color:#191919;
    }
    a  { color:#0000FF; }

	h1 {
     width:100%;
	   text-align: center;
	}
  
  button {
    background-color: #4CAF50;
    color: white;
    padding: 14px 20px;
    border: none;
    cursor: pointer;
  }
        
  .button1{
    border-radius: 50%;
    background-color: #191919;}
  button1:hover {
    opacity: 0.8;
  }
  
  button:hover {
    opacity: 0.8;
  }
        
 div.content {
  padding: 1px 16px;
} 
.center {
	display: flex;
	justify-content: center;
	align-items: center;
}

  
  .left {
    padding: 16px;
    float:left;
    width:50%;
    }
  
  .right {
    padding: 16px;
    }
  
  .err {
    color:red;
    display: flex;
    justify-content: center;
    align-items: center;
  }
  
  .success {
    color:green;
    display: flex;
    justify-content: center;
    align-items: center;
  }
  
.rect {
    background-color: #262626;
    color: white;
    padding: 25px ;
    display: flex;
    flex-direction: column;
    justify-content: center; 
    align-items: center;
    border: 2px solid black;
    cursor: pointer;
    height: 12rem;
    width: 30rem;
}    

.rect_1 {
    background-color: #262626;
    color: white;
    padding: 25px ;
    display: flex;
    justify-content: center; 
    align-items: center;
    border: 2px solid black;
    cursor: pointer;
    height: 3rem;
    width: 30rem;
} 
optgroup{font-size:20px;}
 /* Change styles for span and cancel button on extra small screens */
  @media screen and (max-width: 300px) {
    span.psw {
       display: block;
       float: none;
    }
    .cancelbtn {
       width: 100%;
    }
  }
  
@media screen and (max-width: 700px) {
  .sidebar {
    width: 100%;
    height: auto;
    position: relative;
  }
  .sidebar a {float: left;}
  div.content {margin-left: 0;}
}

@media screen and (max-width: 400px) {
  .sidebar a {
    text-align: center;
    float: none;
  }
}
    </style>
    <!--[if IE]>
    <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->
  </head>
  <body>
<script>
function updateClock ( )
 {
     var currentTime = new Date ( );
     var currentHours = currentTime.getHours ( );
     var currentMinutes = currentTime.getMinutes ( );
     // Pad the hours, minutes and seconds with leading zeros, if required
     currentMinutes = ( currentMinutes < 10 ? "0" : "" ) + currentMinutes;
     if(currentHours > 12) currentHours = currentHours - 12;
     currentHours = (currentHours < 10 ? "0" : "" ) + currentHours;
     var currentTimeString = currentHours + ":" + currentMinutes;

     $("#clock").html(currentTimeString);
          }
 $(document).ready(function() {
    setInterval('updateClock()', 1000);
 });
</script>
<div class="content">
  <h1>
    <form action="" method="post">
      Set Alarm
    </form>
      
  </h1>
  <form action="" method="post">
      <div class = "center">
          <div class = "rect_1">
    <select name="hours" style = "font-size: 1.3rem; width: 5.2rem">
    <optgroup>
      <option value="00">12 AM</option>
      <option value="01">1 AM</option>
      <option value="02">2 AM</option>
      <option value="03">3 AM</option>
      <option value="04">4 AM</option>
      <option value="05">5 AM</option>
      <option value="06">6 AM</option>
      <option value="07">7 AM</option>
      <option value="08">8 AM</option>
      <option value="09">9 AM</option>
      <option value="10">10 AM</option>
      <option value="11">11 AM</option>
      <option value="12">12 PM</option>
      <option value="13">1 PM</option>
      <option value="14">2 PM</option>
      <option value="15">3 PM</option>
      <option value="16">4 PM</option>
      <option value="17">5 PM</option>
      <option value="18">6 PM</option>
      <option value="19">7 PM</option>
      <option value="20">8 PM</option>
      <option value="21">9 PM</option>
      <option value="22">10 PM</option>
      <option value="23">11 PM</option>
    </optgroup>
    </select>
    <label style = "font-size: 1.5rem;">:</label>
    <select name="minutes" style = "width: 5.2postrem; font-size: 1.3rem;">
    <optgroup>
      <option value="00">00</option>
      <option value="05">05</option>
      <option value="10">10</option>
      <option value="15">15</option>
      <option value="20">20</option>
      <option value="25">25</option>
      <option value="30">30</option>
      <option value="35">35</option>
      <option value="40">40</option>
      <option value="45">45</option>
      <option value="50">50</option>
      <option value="55">55</option>
    </optgroup>
    </select>

          </div>
    </div>
<div class = "center">
      <div class = "rect" style = "margin-top: 1%;margin-bottom: 1%;">
          <div>
              <label style = "width:45%">Song</label>
		<p style = "font-size: 0.5rem;"></p>
              <input style = "width: 12rem; height: 2rem;" name="song" required >
              <p sytle = "font-size: 0.5rem;"></p>
          </div>
          <div>
              <label>Artist</label> 
		<p style = "font-size: 0.5rem;"></p>
              <input style = "width: 12rem; height: 2rem;" type="text" name="artist">
          </div>
    </div>
</div>
      <button class = "center" type = "submit" name = "add" style = "margin: auto; width: 22rem; font-size: 1.6rem">Submit</button>
    <div style = "font-size: 1.4rem;"
        <?php
                if (is_null($success)) {
                  echo ">";
                }
                else {
                  if ($success) {
                    echo "class=\"success\"> alarm is set";
                  }

                  else {
                    echo "class=\"err\"> failed";
                  }
                }
              ?>
    </div>
  </form>
</div>
  </body>
</html>
