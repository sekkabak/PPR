<?php
    function send_to_3($param) {

        $req = xmlrpc_encode_request(
            "recvv", 
            [$param]
        );

        $ctx = stream_context_create(
            array(
                'http' => array(
                    'method' 	=> "POST",
                    'header' 	=> array( "Content-Type: text/xml" ),
                    'content' 	=> $req
                )
            )
        );
        #-------------------------------------------------------------------
        $xml = file_get_contents( "http://127.0.0.1:10003/RPC2", false, $ctx );
        #-------------------------------------------------------------------
        $res = xmlrpc_decode( $xml );
        #-------------------------------------------------------------------
        if( $res && xmlrpc_is_fault( $res ) ){
            error_log("xmlrpc: $res[faultString] ($res[faultCode])");
            exit( 1 );
        } else {
            // jest ok
        }
        #===================================================================
    }
    

    /* odbior */
    $xml  = file_get_contents('php://input');
	$params = xmlrpc_decode( $xml );
	$method = basename( $_SERVER[ 'SCRIPT_FILENAME' ] );
	$method = substr( $method, 0, -4 );
	$array = array( 
		'method' => $method,
		'params' =>	$params[0],
	);
    
    $res = xmlrpc_encode_request(NULL, $array);
    print $res;
    send_to_3($array['params']);
?>
