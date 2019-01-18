<!DOCTYPE html>
<html>
<head>
<title>SakilaDB customer editor</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<!-- Bootstrap -->
<link href="../css/bootstrap.min.css" rel="stylesheet">
<!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
<!-- WARNING: Respond.js doesn't work if you view the pagevia file:// -->
<!--[if lt IE 9]>
<script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
<script src="https://oss.maxcdn.com/libs/respond.js/1.3.0/respond.min.js"></script>
<![endif]-->
</head>
<body>
     <nav class="navbar navbar-dark bg-dark">
        <a class="navbar-brand" style="color:white;">Search artists in chinook</a>
        <form class="form-inline" action="../php/search.php" method="get">
            <input class="form-control mr-sm-2" type="search" placeholder="Search Artist" aria-label="Search" name="artist">
            <button class="btn btn-outline-info my-2 my-sm-0" type="submit">Search</button>
        </form>
    </nav>

    <div class="container">
    <?php
        try {
            $conn = new PDO("mysql:host=localhost;dbname=chinook", "root", "");
            $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            $sql = "SELECT Title, AlbumId
                    FROM album 
                    WHERE ArtistId = (
                        SELECT ArtistId 
                        FROM artist 
                        WHERE Name = ?
                   )";

            $stmt = $conn->prepare($sql);
            $stmt->execute([$_GET['artist']]);

            $i = 0;
            while($row = $stmt->fetch()) {
                $title = $row['Title'];
                
                echo
                "
                <table class=\"table table-striped\" style=\"margin-top: 5%\">
                    <thead class=\"thead-dark\">
                    <th scope=\"col\">$title</th>
                    </thead>
                <thead class=\"thead-light\">
                    <th scope=\"col\">#</th>
                    <th scope=\"col\">TrackId</th>
                    <th scope=\"col\">Name</th>
                </thead>
                ";

                $albumId = $row['AlbumId'];
                
                $sql = "SELECT TrackId, Name 
                        FROM track 
                        WHERE AlbumId = $albumId";

                $pippo = $conn->prepare($sql);
                $pippo->execute();

                echo "<tbody>";
                while($track = $pippo->fetch()) {
                    echo
                    "
                    <tr>
                      <th scope=\"col\">0</th>
                      <tr scope=\"col\">$track[0]</tr>
                      <tr scope=\"col\">$track[1]</tr>
                    </tr>
                    ";
                }

                echo "</tbody></table>";
            }

            $conn = null;
        } catch (PDOException $e) {
            $res = false;
            echo $e;
        }
    ?>
    </div>
<!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
<script src="https://code.jquery.com/jquery.js"></script>
<!-- Include all compiled plugins (below), or include individual files as needed -->
<script src="../js/bootstrap.min.js"></script>
</body>
</html>
