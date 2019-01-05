-- phpMyAdmin SQL Dump
-- version 4.8.3
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Nov 09, 2018 at 05:25 PM
-- Server version: 10.1.35-MariaDB
-- PHP Version: 7.2.9

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `thongtinsinhvien`
--
CREATE DATABASE IF NOT EXISTS `thongtinsinhvien` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `thongtinsinhvien`;

DELIMITER $$
--
-- Procedures
--
CREATE DEFINER=`root`@`localhost` PROCEDURE `getSVHaveMaxScore` ()  NO SQL
BEGIN
SELECT s.hoten as sv_maxscore FROM sinhvien s WHERE s.diemtb=(SELECT MAX(diemtb) FROM sinhvien);
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `getSVHaveScoreMoreThan8` ()  NO SQL
BEGIN
SELECT s.hoten,s.diemtb FROM sinhvien s WHERE s.diemtb >8 ORDER BY diemtb DESC;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `getSVHaveScoreMoreThanAVG` ()  NO SQL
BEGIN 
SELECT s.hoten FROM sinhvien s WHERE s.diemtb>(SELECT AVG(diemtb) FROM sinhvien);
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `svcochucvu` ()  NO SQL
BEGIN
SELECT s.hoten FROM sinhvien s WHERE s.machucvu!="CV102";
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `svkhongchucvu` ()  NO SQL
BEGIN
SELECT hoten FROM sinhvien WHERE machucvu="CV102";
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `sv_chucvu` ()  NO SQL
BEGIN
SELECT s.hoten,c.chucvu FROM sinhvien s JOIN chucvu c ON s.machucvu=c.machucvu;
END$$

--
-- Functions
--
CREATE DEFINER=`root`@`localhost` FUNCTION `getSVHaveMaxScore` (`masv` INT) RETURNS VARCHAR(255) CHARSET latin1 NO SQL
BEGIN
DECLARE hoten VARCHAR(255);
SELECT s.hoten INTO hoten FROM sinhvien s WHERE s.diemtb=(SELECT MAX(diemtb) FROM sinhvien) AND s.masv=masv;
RETURN hoten;
END$$

CREATE DEFINER=`root`@`localhost` FUNCTION `getSVHaveScoreMoreThanAVG` (`masv` INT) RETURNS VARCHAR(255) CHARSET latin1 NO SQL
BEGIN
DECLARE hoten VARCHAR(255);
SELECT s.hoten INTO hoten FROM sinhvien s WHERE s.diemtb>(SELECT AVG(diemtb) FROM sinhvien) AND s.masv=masv;
RETURN hoten;
END$$

CREATE DEFINER=`root`@`localhost` FUNCTION `svcochucvu` (`masv` INT(11)) RETURNS VARCHAR(255) CHARSET latin1 NO SQL
BEGIN
DECLARE hoten VARCHAR(255);
SELECT s.hoten INTO hoten FROM sinhvien s WHERE s.machucvu!="CV102" AND masv=s.masv ;
RETURN hoten;
END$$

CREATE DEFINER=`root`@`localhost` FUNCTION `svkhongchucvu` (`masv` INT) RETURNS VARCHAR(255) CHARSET latin1 NO SQL
BEGIN
DECLARE hoten VARCHAR(255);
SELECT s.hoten INTO hoten FROM sinhvien s WHERE s.machucvu="CV102" AND s.masv=masv;
RETURN hoten;
END$$

CREATE DEFINER=`root`@`localhost` FUNCTION `sv_chucvu` (`hoten` VARCHAR(255)) RETURNS VARCHAR(255) CHARSET latin1 NO SQL
BEGIN
DECLARE result VARCHAR(255);
SELECT c.chucvu INTO result FROM chucvu c INNER JOIN sinhvien s ON s.machucvu=c.machucvu WHERE s.hoten=hoten;
RETURN result;
END$$

CREATE DEFINER=`root`@`localhost` FUNCTION `updateSVHaveMinScore` () RETURNS VARCHAR(255) CHARSET latin1 NO SQL
BEGIN
UPDATE `sinhvien` SET `diemtb`=diemtb +1 WHERE diemtb=(SELECT MIN(diemtb) FROM (SELECT diemtb FROM sinhvien) as diemTB);
RETURN "Finish";
END$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `chucvu`
--

CREATE TABLE IF NOT EXISTS `chucvu` (
  `machucvu` varchar(100) NOT NULL,
  `chucvu` varchar(255) NOT NULL,
  PRIMARY KEY (`machucvu`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `chucvu`
--

INSERT INTO `chucvu` (`machucvu`, `chucvu`) VALUES
('CV100 ', 'Lop truong'),
('CV101', 'Bi thu'),
('CV102', 'Sinh vien');

-- --------------------------------------------------------

--
-- Table structure for table `khoa`
--

CREATE TABLE IF NOT EXISTS `khoa` (
  `makhoa` varchar(150) NOT NULL,
  `tenkhoa` varchar(255) NOT NULL,
  PRIMARY KEY (`makhoa`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `khoa`
--

INSERT INTO `khoa` (`makhoa`, `tenkhoa`) VALUES
('CN100', 'Cong nghe thong tin'),
('CN101', 'Mang may tinh'),
('CN102', 'Khoa hoc may tinh');

-- --------------------------------------------------------

--
-- Table structure for table `sinhvien`
--

CREATE TABLE IF NOT EXISTS `sinhvien` (
  `masv` int(11) NOT NULL,
  `hoten` varchar(255) NOT NULL,
  `machucvu` varchar(100) NOT NULL,
  `diachi` text NOT NULL,
  `gioitinh` varchar(10) NOT NULL,
  `diemtb` double NOT NULL,
  `makhoa` varchar(150) NOT NULL,
  PRIMARY KEY (`masv`),
  KEY `machucvu` (`machucvu`),
  KEY `makhoa` (`makhoa`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `sinhvien`
--

INSERT INTO `sinhvien` (`masv`, `hoten`, `machucvu`, `diachi`, `gioitinh`, `diemtb`, `makhoa`) VALUES
(1000, 'Thi Quy', 'CV100 ', '123 yen hoa', 'Nam', 3.1, 'CN100'),
(1001, 'Van Quy', 'CV101', '12 yen hoa', 'Nam', 2.2, 'CN101'),
(1002, 'Hoang Quy', 'CV102', '43 cau giay', 'Nu', 4.4, 'CN102'),
(1003, 'Duy Ninh', 'CV100 ', '323 yen hoa', 'Nu', 5.5, 'CN102'),
(1004, 'Duy Linh', 'CV100 ', '123 cau giay', 'Nu', 6.6, 'CN100'),
(1005, 'Thanh', 'CV101', '123 cau giay', 'Nam', 4.3, 'CN100'),
(1006, 'Thang', 'CV102', '123 cau giay', 'Nam', 9.8, 'CN100'),
(1007, 'Tung', 'CV100 ', '123 cau giay', 'Nu', 7.6, 'CN101'),
(1008, 'Thinh', 'CV102', '123 cau giay', 'Nu', 5.7, 'CN101'),
(1009, 'Minh', 'CV100 ', '123 cau giay', 'Nam', 5.9, 'CN100'),
(1010, 'Ha', 'CV101', '123 cau giay', 'Nu', 3.3, 'CN100'),
(1011, 'Son', 'CV100 ', '123 cau giay', 'Nam', 6.3, 'CN102'),
(1012, 'Hanh', 'CV102', '123 cau giay', 'Nam', 8.5, 'CN100'),
(1013, 'Linh', 'CV100 ', '123 cau giay', 'Nu', 9, 'CN100'),
(1014, 'Phuc', 'CV100 ', '123 cay giay', 'Nam', 7.2, 'CN101'),
(1015, 'Nguyen Thang', 'CV101', '534 cau giay', 'Nam', 5.7, 'CN102'),
(1016, 'Viet Anh', 'CV102', '34 cau giay', 'Nam', 10, 'CN100'),
(1017, 'Tam', 'CV101', '89 cau giay', 'Nu', 4.3, 'CN100'),
(1018, 'Dam Thanh', 'CV100 ', '14 cau giay', 'Nam', 8.8, 'CN100'),
(1019, 'Phi Son', 'CV101', '94 cau giay', 'Nam', 6.6, 'CN102');

--
-- Constraints for dumped tables
--

--
-- Constraints for table `sinhvien`
--
ALTER TABLE `sinhvien`
  ADD CONSTRAINT `sinhvien_ibfk_1` FOREIGN KEY (`machucvu`) REFERENCES `chucvu` (`machucvu`),
  ADD CONSTRAINT `sinhvien_ibfk_2` FOREIGN KEY (`makhoa`) REFERENCES `khoa` (`makhoa`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
