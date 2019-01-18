<?php
  try {
    // Apertura della connessione
    $conn = new PDO("mysql:host=localhost;dbname=sakila", "root", "");
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    if(!isset($_GET["active"])) $_GET["active"] = 0;

    // Query di inserimento dei dati ricevuti tramite GET
    $sql = "INSERT INTO customer (store_id, first_name, last_name, email, address_id, active, create_date) VALUES(?, ?, ?, ?, ?, ?, ?)";
    $stmt = $conn->prepare($sql);
    $stmt->execute([$_GET["store_id"], $_GET["first_name"], $_GET["last_name"], $_GET["email"], $_GET["address_id"], $_GET["active"], '1970-1-1']);

    // Prelevamento dell'id del record appena inserimento
    $newRecordID = $conn->lastInsertId();

    // Query di selezione del record appena inserito nel database
    $sql = "SELECT * FROM customer WHERE customer_id = ?";
    $stmt = $conn->prepare($sql);
    $stmt->execute([$newRecordID]);

    // Prelevamento del risultato della query
    $newRecord = $stmt->fetch();

    // Creazione di un JSON contenente le informazioni del record prelevato
    $selectedRow = array(
      'success' => true,
      'customer_id' => $newRecord[0],
      'store_id' => $newRecord[1],
      'first_name' => $newRecord[2],
      'last_name' => $newRecord[3],
      'email' => $newRecord[4],
      'address_id' => $newRecord[5],
      'active' => $newRecord[6],
      'create_date' => $newRecord[7],
      'last_update' => $newRecord[8]
    );

    // Invio del JSON al client
    echo json_encode($selectedRow);

  } catch (PDOException $e) {
    // Creazione di un JSON contenente le informazioni di errore
    $errorRes = array('success' => false,
                      'error' => $e->getMessage());
    // Invio del JSON al client
    echo json_encode($errorRes);
  }

  // Chiusura della connessione
  $conn = null;
?>
