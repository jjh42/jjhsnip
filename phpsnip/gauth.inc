<?php
require_once "HTTP/Request.php";

function get_google_login($emailad, $passwd, $service) {
    $gloginurl="https://www.google.com/accounts/ClientLogin";

    $req = & new HTTP_Request($gloginurl);
    $req->addHeader("Content-Type", "application/x-www-form-urlencoded");
    $req->setMethod(HTTP_REQUEST_METHOD_POST);
    $req->addPostData("Email", $emailad);
    $req->addPostData("Passwd", $passwd);
    $req->addPostData("service", $service);
    $req->addPostData("source", "quarks-mybooks-0.1");
    $req->addPostData("accountType", "GOOGLE");

    if (PEAR::isError($req->sendRequest())) {
	die("Error logging into google with error " . $req->getResposneCode() . "<br>");
    }
    	
    // Need to read the google response
    if ($req->getResponseCode() != 200) {
	echo "Error authenticating\n<br>";
	echo $req->getResponseBody();
    }
    
    // Now we need to extrac the auth key
    $body = $req->getResponseBody();
    $sres = preg_match("/Auth=(.*)/", $body, $matches);

    if($sres == 0) {
        die("Error logging into google 2.");
    }
    $key = $matches[1];

    return $key;
}


?>
