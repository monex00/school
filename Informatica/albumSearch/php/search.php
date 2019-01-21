<?php
$res;
try {
    // Apertura della comunicazione
    $conn = new PDO("mysql:host=localhost;dbname=chinook", "root", "");
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Selezione degli album di un determinato gruppo
    $sql = "SELECT Title, AlbumId
            FROM album
            WHERE ArtistId = (
                SELECT ArtistId
                FROM artist
                WHERE Name = ?
           )";

    // Esecuzione della query
    $stmt = $conn->prepare($sql);
    $stmt->execute([$_GET['artist']]);

    $res = array();

    // Ciclo per ciascun album
    $i = 0;
    while($albumRow = $stmt->fetch()) {
        // Prelevamento dal record delle informazioni significative dell'album
        $albumTitle = $albumRow['Title'];
        $albumId = $albumRow['AlbumId'];

        // Aggiunta del nome dell'album al json di risultato
        $res['Artist'] = $_GET['artist'];
        $res['Albums'][$i]['Title'] = $albumTitle;

        // Query di ricerca delle tracce di un determinato album
        $sql = "SELECT TrackId, Name
                FROM track
                WHERE AlbumId = $albumId";

        // Esecuzione della query
        $pippo = $conn->prepare($sql);
        $pippo->execute();

        // Ciclo per ciascuna traccia
        $j = 0;
        while($track = $pippo->fetch()) {
            // Aggiunta della traccia all'album
            $res['Albums'][$i]['Tracks'][$j] = $track['Name'];
            $j++;
        }
        $i++;
    }
} catch (PDOException $e) {
    $res ['Error'] = $e->getMessage();
} catch (Exception $e) {
    $res['Error'] = $e->getMessage();
} finally {
    echo json_encode($res);
    $conn = null;
}
?>
