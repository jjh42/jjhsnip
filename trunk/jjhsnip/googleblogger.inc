<?php
require_once("HTTP/Request.php");


function publish_to_blog($head, $content, $labels, $blogid, $gauth, $name, $email)
// Publish the post with heading, content and labels to Blogger (Beta).
{    

   $xml = "<entry xmlns=\"http://www.w3.org/2005/Atom\">
         <category  scheme=\"http://www.blogger.com/atom/ns#\" term=\"" . $labels . "\" />
         <title type=\"text\">" . $head  . "</title>
            <content type=\"xhtml\">
                <div xmlns=\"http://www.w3.org/1999/xhtml\">" . $content . "</div>
            </content>            
            <author>
            <name>".$name ."</name>
            <email>".$email."</email>
            </author>
        </entry>";

    $url = "http://www.blogger.com/feeds/".$blogid."/posts/default";

    // Allow for redirects
    // Okay now build the post
    $req = & new HTTP_Request($url, array('allowRedirects' => true));
    $req->addHeader("Authorization", "GoogleLogin Auth=" . $gauth);
    $req->addHeader("Content-Type", "application/atom+xml");
    $req->setMethod(HTTP_REQUEST_METHOD_POST);
    $req->addRawPostData($xml, true);

    $reqstr = $req->_buildRequest();
    $resp = $req->sendRequest();
    if (PEAR::isError($resp)) {
        die("Error logging into google - error " . $req->getResponseCode());
    }
    	
    // Need to read the google response
    if ($req->getResponseCode() != 201) {
	echo "Error publishing\n<br>";
        echo "Code was ". $req->getResponseCode();
	echo $req->getResponseBody();
    }

}

?>
