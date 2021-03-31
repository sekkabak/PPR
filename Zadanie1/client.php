#!/usr/bin/php

<?php
	$host = "127.0.0.1";
	$port = 12345;

	if( ! ( $client = socket_create( AF_INET, SOCK_STREAM, SOL_TCP ) ) ){
		print "socket_create(): " 	. socket_strerror( socket_last_error( $client ) ) . "\n";
		exit( 1 );
	}
	if( ! socket_connect( $client, $host, $port ) ){
		print "socket_connect(): " 	. socket_strerror( socket_last_error( $client ) ) . "\n";
		exit( 1 );
	}
		
	$mssg = readline('Podaj ciąg do przekazania: ');
	socket_write($client, $mssg, strlen($mssg));
	
	socket_close( $client );
?>
