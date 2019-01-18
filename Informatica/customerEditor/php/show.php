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
  <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
    <a class="navbar-brand" href="index.html">SakilaDB</a>
    <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
      <span class="navbar-toggler-icon"></span>
    </button>
    <div class="collapse navbar-collapse" id="navbarNav">
      <ul class="navbar-nav">
        <li class="nav-item">
          <a class="nav-link" href="../html/add.html">Add <span class="sr-only">(current)</span></a>
        </li>
        <li class="nav-item">
          <a class="nav-link" href="../html/remove.html">Remove</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" href="../html/modify.html">Modify</a>
        </li>
        <li class="nav-item">
          <a class="nav-link active" href="../php/show.php">Show</a>
        </li>
      </ul>
    </div>
  </nav>
<table class="table table-striped" >
  <thead class="thead-dark">
    <th scope="col">#</th>
    <th scope="col">customer_id</th>
    <th scope="col">store_id</th>
    <th scope="col">first_name</th>
    <th scope="col">last_name</th>
    <th scope="col">email</th>
    <th scope="col">address_id</th>
    <th scope="col">active</th>
    <th scope="col">create_date</th>
    <th scope="col">last_update</th>
    <th scope="col">store_id</th>
    <th scope="col">manger_staff_id</th>
    <th scope="col">address_id</th>
    <th scope="col">last_update</th>
  </thead>
  <tbody>
  <?php
    try {
      $conn = new PDO("mysql:host=localhost;dbname=sakila", "root", "");
      $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
      $res = true;

      $stmt = $conn->query("SELECT * from customer JOIN store ON customer.store_id = store.store_id");

      $i = 0;
      while($row = $stmt->fetch()) {
        echo
        "
        <tr>
          <th scope=\"col\">$i</th>
          <th scope=\"col\">$row[0]</th>
          <th scope=\"col\">$row[1]</th>
          <th scope=\"col\">$row[2]</th>
          <th scope=\"col\">$row[3]</th>
          <th scope=\"col\">$row[4]</th>
          <th scope=\"col\">$row[5]</th>
          <th scope=\"col\">$row[6]</th>
          <th scope=\"col\">$row[7]</th>
          <th scope=\"col\">$row[8]</th>
          <th scope=\"col\">$row[9]</th>
          <th scope=\"col\">$row[10]</th>
          <th scope=\"col\">$row[11]</th>
          <th scope=\"col\">$row[12]</th>
        </tr>
        ";
        $i++;
      }

      $conn = null;
    } catch (PDOException $e) {
      $res = false;
    }
   ?>
 </tbody>
</table>
<!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
<script src="https://code.jquery.com/jquery.js"></script>
<!-- Include all compiled plugins (below), or include individual files as needed -->
<script src="../js/bootstrap.min.js"></script>
</body>
</html>
