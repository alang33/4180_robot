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
        $txt_2 = "$hours-$minutes\n";
        if(isset($_POST["add"])){
            $file = fopen("/var/www/html/song.txt", "r");
            $file_2 = fopen("/var/www/html/alarm.txt", "r");
            $status = fread($file,4);
            $status_2 = fread($file_2,4);       
            fclose($file);
            fclose($file_2);
            if ($status == "NULL"){
              $file = fopen("/var/www/html/song.txt", "w");
              fwrite($file, $txt);
              fclose($file);
            }
            if ($status_2 == "NULL"){
                          $file_2 = fopen("/var/www/html/alarm.txt", "w");
                          fwrite($file_2, $txt_2);
                          fclose($file_2);
                        }
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
      font-family: 'Strait';font-size: 22px;
      background-color:#191919;
    }
    a  { color:#0000FF; }

	h1 {
     width:100%;
	   margin:10px;
	   text-align: center;
	   float:right;
	}
	

  
  button {
    background-color: #4CAF50;
    color: white;
    padding: 14px 20px;
    margin: 8px 0;
    border: none;
    cursor: pointer;
    width: 100%;
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
  margin-left: 200px;
  padding: 1px 16px;
  height: 800px;
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
  }
  
  .success {
    color:green;
  }
  
.rect {
    background-color: #262626;
    color: white;
    padding: 25px ;
    margin-left: 30%;
    margin-top: 25%;
    border: 2px solid black;
    cursor: pointer;
    height: 100px;
    width: 35%;
}    

.rect_1 {
    background-color: #262626;
    color: white;
    padding: 25px ;
    margin-left: 30%;
    margin-top: 25%;
    border: 2px solid black;
    cursor: pointer;
    height: 25px;
    width: 35%;
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
      <div class = "rect_1" style = "margin-top: 5%; margin-bottom: 1%;">
          <div>
    <select name="hours" style = "margin-top: 0%;margin-left: 5%;width:40%; height: 150%; font-size: 20px;">
    <optgroup>
      <option value="0">0</option>
      <option value="1">1</option>
      <option value="2">2</option>
      <option value="3">3</option>
      <option value="4">4</option>
      <option value="5">5</option>
      <option value="6">6</option>
      <option value="7">7</option>
      <option value="8">8</option>
      <option value="9">9</option>
      <option value="10">10</option>
      <option value="11">11</option>
      <option value="12">12</option>
    </optgroup>
    </select>
    <label>:</label>
    <select name="minutes" style = "margin-top: 0%; margin-left: 5%;width:40%; height: 150%; font-size: 20px;">
    <optgroup>
      <option value="0">0</option>
      <option value="5">5</option>
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
      <div class = "rect" style = "margin-top: 1%;margin-bottom: 1%;">
          <div>
              <label style = "width:45%">Song</label>
              <input style = "width:45%; height: 30%; margin-left: 5%;" type="text" name="song" required >
          </div>
          <div>
              <label>Artist</label>
              <input style = "width:45%; height: 30%; margin-top: 5%; margin-left: 5%;" type="text" name="artist">
          </div>
    </div>
      <button type = "submit" name = "add" style = "margin-top: 1%; margin-left: 33%;width:25%; font-size: 15px">Submit</button>
    <div style = "margin-left:35%;"
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
