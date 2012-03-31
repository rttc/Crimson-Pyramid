-- MySQL dump 10.13  Distrib 5.1.61, for redhat-linux-gnu (i386)
--
-- Host: localhost    Database: rttc_cp
-- ------------------------------------------------------
-- Server version	5.1.61

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `backup_servers`
--

DROP TABLE IF EXISTS `backup_servers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `backup_servers` (
  `srvid` int(10) unsigned NOT NULL,
  `bsrvid` int(10) unsigned NOT NULL,
  `options` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`srvid`),
  KEY `bsrvid` (`bsrvid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `backup_servers`
--

LOCK TABLES `backup_servers` WRITE;
/*!40000 ALTER TABLE `backup_servers` DISABLE KEYS */;
/*!40000 ALTER TABLE `backup_servers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `backups`
--

DROP TABLE IF EXISTS `backups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `backups` (
  `fileid` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `ts_backup` bigint(20) unsigned NOT NULL,
  `ts_modified` bigint(20) unsigned NOT NULL,
  `ts_lastretr` bigint(20) unsigned NOT NULL DEFAULT '0',
  `srvid` int(10) unsigned NOT NULL,
  `origpath` mediumtext NOT NULL,
  `locale` int(10) unsigned NOT NULL,
  `hash` varchar(65) DEFAULT NULL,
  `owner` bigint(20) unsigned NOT NULL DEFAULT '0',
  `fsize` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`fileid`),
  KEY `srvid` (`srvid`),
  KEY `owner` (`owner`),
  KEY `locale` (`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `backups`
--

LOCK TABLES `backups` WRITE;
/*!40000 ALTER TABLE `backups` DISABLE KEYS */;
/*!40000 ALTER TABLE `backups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dns_acldata`
--

DROP TABLE IF EXISTS `dns_acldata`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dns_acldata` (
  `aclid` tinyint(3) unsigned NOT NULL,
  `addr` varchar(16) NOT NULL,
  KEY `aclid` (`aclid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dns_acls`
--

DROP TABLE IF EXISTS `dns_acls`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dns_acls` (
  `aclname` varchar(16) NOT NULL,
  `aclid` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`aclid`),
  UNIQUE KEY `aclname` (`aclname`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dns_domains`
--

DROP TABLE IF EXISTS `dns_domains`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dns_domains` (
  `domain` varchar(64) NOT NULL,
  `domid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`domid`),
  UNIQUE KEY `domain` (`domain`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dns_domains`
--

LOCK TABLES `dns_domains` WRITE;
/*!40000 ALTER TABLE `dns_domains` DISABLE KEYS */;
/*!40000 ALTER TABLE `dns_domains` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dns_entries`
--

DROP TABLE IF EXISTS `dns_entries`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dns_entries` (
  `domid` int(10) unsigned NOT NULL,
  `name` varchar(64) NOT NULL,
  `type` enum('A','AAAA','CNAME','TXT','MX','NS','SRV') NOT NULL DEFAULT 'TXT',
  `ip4` varchar(16) DEFAULT NULL,
  `ip6` varchar(128) DEFAULT NULL,
  `host` varchar(64) DEFAULT NULL,
  KEY `domid` (`domid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dns_entries`
--

LOCK TABLES `dns_entries` WRITE;
/*!40000 ALTER TABLE `dns_entries` DISABLE KEYS */;
/*!40000 ALTER TABLE `dns_entries` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dns_sd`
--

DROP TABLE IF EXISTS `dns_sd`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dns_sd` (
  `domid` int(10) unsigned NOT NULL,
  `srvid` int(10) unsigned NOT NULL,
  `role` enum('M','S') NOT NULL DEFAULT 'M',
  KEY `domid` (`domid`),
  KEY `srvid` (`srvid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dns_sd`
--

LOCK TABLES `dns_sd` WRITE;
/*!40000 ALTER TABLE `dns_sd` DISABLE KEYS */;
/*!40000 ALTER TABLE `dns_sd` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dns_srv`
--

DROP TABLE IF EXISTS `dns_srv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dns_srv` (
  `servername` varchar(64) NOT NULL,
  `srvid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`srvid`),
  UNIQUE KEY `servername` (`servername`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `restore_delreqs`
--

DROP TABLE IF EXISTS `restore_delreqs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `restore_delreqs` (
  `fileid` bigint(20) unsigned NOT NULL,
  `ts_requested` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`fileid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `restore_delreqs`
--

LOCK TABLES `restore_delreqs` WRITE;
/*!40000 ALTER TABLE `restore_delreqs` DISABLE KEYS */;
/*!40000 ALTER TABLE `restore_delreqs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `restore_requests`
--

DROP TABLE IF EXISTS `restore_requests`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `restore_requests` (
  `fileid` bigint(20) unsigned NOT NULL,
  `ts_requested` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`fileid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `restore_requests`
--

LOCK TABLES `restore_requests` WRITE;
/*!40000 ALTER TABLE `restore_requests` DISABLE KEYS */;
/*!40000 ALTER TABLE `restore_requests` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sessions`
--

DROP TABLE IF EXISTS `sessions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sessions` (
  `uid` bigint(20) unsigned NOT NULL,
  `ipaddr` varchar(16) DEFAULT NULL,
  `expire` bigint(20) unsigned NOT NULL,
  `initiated` bigint(20) unsigned NOT NULL,
  `hash` varchar(32) NOT NULL,
  PRIMARY KEY (`hash`),
  KEY `uid` (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sessions`
--

LOCK TABLES `sessions` WRITE;
/*!40000 ALTER TABLE `sessions` DISABLE KEYS */;
/*!40000 ALTER TABLE `sessions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sql_create`
--

DROP TABLE IF EXISTS `sql_create`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sql_create` (
  `uid` bigint(20) unsigned NOT NULL,
  `ts_submitted` bigint(20) unsigned NOT NULL DEFAULT '1',
  `name` varchar(32) NOT NULL,
  `srvid` int(10) unsigned NOT NULL,
  KEY `uid` (`uid`),
  KEY `srvid` (`srvid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sql_dbs`
--

DROP TABLE IF EXISTS `sql_dbs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sql_dbs` (
  `name` varchar(128) NOT NULL,
  `owner` bigint(20) unsigned NOT NULL,
  `ts_created` bigint(20) unsigned NOT NULL DEFAULT '1',
  `srvid` int(10) unsigned NOT NULL,
  KEY `owner` (`owner`),
  KEY `srvid` (`srvid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sql_dbs`
--

LOCK TABLES `sql_dbs` WRITE;
/*!40000 ALTER TABLE `sql_dbs` DISABLE KEYS */;
/*!40000 ALTER TABLE `sql_dbs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sql_destroy`
--

DROP TABLE IF EXISTS `sql_destroy`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sql_destroy` (
  `uid` bigint(20) unsigned NOT NULL,
  `ts_submitted` bigint(20) unsigned NOT NULL DEFAULT '1',
  `name` varchar(32) NOT NULL,
  `srvid` int(10) unsigned NOT NULL,
  KEY `uid` (`uid`),
  KEY `srvid` (`srvid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sql_destroy`
--

LOCK TABLES `sql_destroy` WRITE;
/*!40000 ALTER TABLE `sql_destroy` DISABLE KEYS */;
/*!40000 ALTER TABLE `sql_destroy` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `unix_assocweb`
--

DROP TABLE IF EXISTS `unix_assocweb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `unix_assocweb` (
  `dbserver` int(10) unsigned NOT NULL,
  `webserver` int(10) unsigned NOT NULL,
  KEY `dbserver` (`dbserver`),
  KEY `webserver` (`webserver`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `unix_srv`
--

DROP TABLE IF EXISTS `unix_srv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `unix_srv` (
  `srvid` int(10) unsigned NOT NULL,
  `servername` varchar(64) NOT NULL,
  `description` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`srvid`),
  UNIQUE KEY `servername` (`servername`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `unix_srvusr`
--

DROP TABLE IF EXISTS `unix_srvusr`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `unix_srvusr` (
  `srvid` int(10) unsigned NOT NULL,
  `uid` int(10) unsigned NOT NULL,
  `homedir` varchar(64) NOT NULL,
  `shell` varchar(32) NOT NULL,
  KEY `srvid` (`srvid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `unix_srvusr`
--

LOCK TABLES `unix_srvusr` WRITE;
/*!40000 ALTER TABLE `unix_srvusr` DISABLE KEYS */;
/*!40000 ALTER TABLE `unix_srvusr` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `unix_users`
--

DROP TABLE IF EXISTS `unix_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `unix_users` (
  `uid` int(10) unsigned NOT NULL,
  `username` varchar(8) NOT NULL,
  `password` varchar(64) DEFAULT NULL,
  `gecos` varchar(128) NOT NULL,
  `domid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`uid`),
  UNIQUE KEY `username` (`username`),
  KEY `domid` (`domid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `unix_users`
--

LOCK TABLES `unix_users` WRITE;
/*!40000 ALTER TABLE `unix_users` DISABLE KEYS */;
/*!40000 ALTER TABLE `unix_users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `userdomperms`
--

DROP TABLE IF EXISTS `userdomperms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `userdomperms` (
  `domid` int(10) unsigned NOT NULL,
  `uid` bigint(20) unsigned NOT NULL,
  KEY `domid` (`domid`),
  KEY `uid` (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `userdomperms`
--

LOCK TABLES `userdomperms` WRITE;
/*!40000 ALTER TABLE `userdomperms` DISABLE KEYS */;
/*!40000 ALTER TABLE `userdomperms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `uid` bigint(20) unsigned NOT NULL,
  `username` varchar(64) NOT NULL,
  `password` mediumblob,
  `ts_created` int(10) unsigned NOT NULL,
  `ts_lastupdated` int(10) unsigned NOT NULL,
  `admin` enum('E','A','G','N') NOT NULL DEFAULT 'N',
  `email` varchar(64) NOT NULL,
  PRIMARY KEY (`uid`),
  UNIQUE KEY `email` (`email`),
  UNIQUE KEY `username` (`username`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-03-13 15:10:05
