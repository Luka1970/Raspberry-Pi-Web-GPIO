<?php
// Enter MySQLi connection details below.

$MySQLi_Host		= 'localhost';	// Enter your MySQLi Server address here.
$MySQLi_User		= 'user';		// Enter your MySQLi Username here.
$MySQLi_Password	= 'pass';		// Enter your MySQLi Password here.
$MySQLi_DataBase	= 'gpio';		// Enter your MySQLi Database here.

$db = new mysqli($MySQLi_Host, $MySQLi_User, $MySQLi_Password, $MySQLi_DataBase);


// Raspberry Pi board revision
// Enter your revision if you know it, OR
// leave this line default and run setup.py
// eg; python setup.py
$pi_rev = '';
?>
