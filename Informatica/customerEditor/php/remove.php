 <?php
   try {
     // Apertura della connessione
     $conn = new PDO("mysql:host=localhost;dbname=sakila", "root", "");
     $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

     if(!isset($_GET["active"])) $_GET["active"] = 0;

     // Query di eliminazione del record ricevuto tramite GET
     $sql = "DELETE FROM customer WHERE customer_id = ?";
     $stmt = $conn->prepare($sql);
     $stmt->execute([$_GET['customer_id']]);

     // Creazione di un JSON contenente le informazioni del record prelevato
     $result = array('success' => true);

     // Invio del JSON al client
     echo json_encode($result);

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
