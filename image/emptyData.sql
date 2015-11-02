/*
MySQL Backup
Source Server Version: 5.5.37
Source Database: hddpos
Date: 2015/9/1 18:36:57
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
--  Table structure for `cey_bt_class`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_class`;
CREATE TABLE `cey_bt_class` (
  `ch_classno` varchar(2) NOT NULL,
  `vch_classname` varchar(12) NOT NULL,
  `dt_begin` datetime DEFAULT NULL,
  PRIMARY KEY (`ch_classno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_cp_dish`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_cp_dish`;
CREATE TABLE `cey_bt_cp_dish` (
  `ch_dishno` varchar(7) NOT NULL,
  `ch_no` varchar(4) NOT NULL,
  PRIMARY KEY (`ch_dishno`,`ch_no`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_cp_memo`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_cp_memo`;
CREATE TABLE `cey_bt_cp_memo` (
  `ch_no` varchar(4) NOT NULL,
  `vch_cp_memo` varchar(40) NOT NULL,
  `ch_typeno` varchar(2) NOT NULL,
  `num_add_price` decimal(12,2) DEFAULT NULL,
  PRIMARY KEY (`ch_no`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_cp_type`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_cp_type`;
CREATE TABLE `cey_bt_cp_type` (
  `ch_typeno` varchar(2) NOT NULL,
  `vch_typename` varchar(20) NOT NULL,
  PRIMARY KEY (`ch_typeno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_dish`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_dish`;
CREATE TABLE `cey_bt_dish` (
  `ch_dishno` varchar(7) NOT NULL,
  `vch_dishname` varchar(60) NOT NULL,
  `vch_spell` varchar(40) DEFAULT NULL,
  `vch_english` varchar(40) DEFAULT NULL,
  `ch_dish_typeno` varchar(2) DEFAULT NULL,
  `ch_unitno` varchar(2) DEFAULT NULL,
  `num_price` decimal(12,2) DEFAULT '0.00',
  `ch_suitflag` varchar(1) DEFAULT 'N',
  `ch_discount` varchar(1) DEFAULT 'N',
  `ch_curprice` varchar(1) DEFAULT 'N',
  `ch_tempflag` varchar(1) DEFAULT 'N',
  `ch_stopflag` varchar(1) DEFAULT NULL,
  `ch_printflag` varchar(1) DEFAULT NULL,
  `vch_printer` varchar(50) DEFAULT NULL,
  `ch_sub_typeno` varchar(4) DEFAULT NULL,
  `ch_materialno` varchar(7) DEFAULT NULL,
  `ch_serviceflag` varchar(1) DEFAULT NULL,
  `ch_lowflag` varchar(1) DEFAULT NULL,
  `num_m_price` decimal(12,2) DEFAULT '0.00',
  `ch_deductflag` varchar(1) DEFAULT NULL,
  `ch_deducttype` varchar(1) DEFAULT NULL,
  `int_deduct` int(11) DEFAULT NULL,
  `num_deduct` decimal(12,2) DEFAULT NULL,
  `ch_saleflag` varchar(1) DEFAULT NULL,
  `ch_saletype` varchar(1) DEFAULT NULL,
  `int_sale_deduct` int(11) DEFAULT NULL,
  `num_sale` decimal(12,2) DEFAULT NULL,
  `item_flag` varchar(1) DEFAULT NULL,
  `vch_pur_unitno` varchar(2) DEFAULT NULL,
  `int_unit_rate` int(11) DEFAULT NULL,
  `ch_off` varchar(1) DEFAULT 'N',
  `vch_subno` varchar(40) DEFAULT NULL,
  `num_sale_price` decimal(20,2) DEFAULT '0.00',
  `ch_outflag` varchar(1) DEFAULT NULL,
  `vch_outprint` varchar(50) DEFAULT NULL,
  `int_rate` decimal(12,2) DEFAULT NULL,
  `ch_weight` varchar(1) DEFAULT NULL,
  `vch_barcode` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`ch_dishno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_dish_special`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_dish_special`;
CREATE TABLE `cey_bt_dish_special` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `dt_date` datetime NOT NULL,
  `ch_dishno` varchar(7) DEFAULT NULL,
  `num_price` decimal(12,2) DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_operID2` varchar(6) DEFAULT NULL,
  `dt_operdate2` datetime DEFAULT NULL,
  `dt_operdate_effect` datetime DEFAULT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_dish_subtype`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_dish_subtype`;
CREATE TABLE `cey_bt_dish_subtype` (
  `ch_sub_typeno` varchar(4) NOT NULL,
  `vch_sub_typename` varchar(20) NOT NULL,
  `ch_dish_typeno` varchar(2) NOT NULL,
  `ch_labelprint_flag` varchar(1) DEFAULT 'N',
  PRIMARY KEY (`ch_sub_typeno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_dish_suit`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_dish_suit`;
CREATE TABLE `cey_bt_dish_suit` (
  `int_flowid` int(11) NOT NULL AUTO_INCREMENT,
  `ch_suitno` varchar(7) NOT NULL,
  `ch_dishno` varchar(7) DEFAULT NULL,
  `num_num` decimal(12,2) DEFAULT '1.00',
  `ch_dishno_chang` varchar(7) DEFAULT NULL,
  `num_num_change` decimal(12,2) DEFAULT NULL,
  PRIMARY KEY (`int_flowid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_dish_type`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_dish_type`;
CREATE TABLE `cey_bt_dish_type` (
  `ch_dish_typeno` varchar(2) NOT NULL,
  `vch_dish_typename` varchar(20) NOT NULL,
  PRIMARY KEY (`ch_dish_typeno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_dish_warn`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_dish_warn`;
CREATE TABLE `cey_bt_dish_warn` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `dt_date` datetime NOT NULL,
  `ch_dishno` varchar(7) DEFAULT NULL,
  `num_num` decimal(12,2) DEFAULT NULL,
  `num_sale` decimal(12,2) DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_operID2` varchar(6) DEFAULT NULL,
  `dt_operdate2` datetime DEFAULT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_kitchen_print`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_kitchen_print`;
CREATE TABLE `cey_bt_kitchen_print` (
  `int_id` int(32) NOT NULL,
  `vch_printname` varchar(32) DEFAULT NULL,
  `vch_printip` varchar(32) DEFAULT NULL,
  `vch_printport` varchar(32) DEFAULT NULL,
  `vch_printother` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`int_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_paymode`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_paymode`;
CREATE TABLE `cey_bt_paymode` (
  `ch_paymodeno` varchar(2) NOT NULL,
  `vch_paymodename` varchar(60) NOT NULL,
  `ch_IDflag` varchar(1) DEFAULT NULL,
  `ch_faceflag` varchar(1) DEFAULT NULL,
  `ch_changeflag` varchar(1) DEFAULT NULL,
  `ch_incomeflag` varchar(1) DEFAULT NULL,
  `ch_key` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`ch_paymodeno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_project`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_project`;
CREATE TABLE `cey_bt_project` (
  `ch_projectno` varchar(3) NOT NULL,
  `vch_projectname` varchar(30) NOT NULL,
  `ch_dishflag` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`ch_projectno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_project_detail`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_project_detail`;
CREATE TABLE `cey_bt_project_detail` (
  `ch_projectno` varchar(3) NOT NULL,
  `ch_typeno` varchar(4) NOT NULL,
  `int_discount` int(11) NOT NULL,
  PRIMARY KEY (`ch_projectno`,`ch_typeno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_reason`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_reason`;
CREATE TABLE `cey_bt_reason` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `vch_reason` varchar(50) NOT NULL,
  `vch_type` varchar(20) NOT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_suit_change`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_suit_change`;
CREATE TABLE `cey_bt_suit_change` (
  `int_flowID` int(11) NOT NULL,
  `ch_dishno` varchar(7) NOT NULL,
  `num_num_change` decimal(12,2) DEFAULT NULL,
  PRIMARY KEY (`ch_dishno`,`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_table`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_table`;
CREATE TABLE `cey_bt_table` (
  `ch_tableno` varchar(4) NOT NULL,
  `vch_tablename` varchar(12) NOT NULL,
  `ch_typeno` varchar(2) DEFAULT NULL,
  `ch_billno` varchar(15) DEFAULT NULL,
  `ch_lockflag` varchar(1) DEFAULT NULL,
  `int_person` int(11) DEFAULT NULL,
  `vch_memo` varchar(100) DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT NULL,
  `dt_datetime` datetime DEFAULT NULL,
  `ch_areaNo` varchar(2) DEFAULT NULL,
  PRIMARY KEY (`ch_tableno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_table_area`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_table_area`;
CREATE TABLE `cey_bt_table_area` (
  `ch_areano` varchar(2) NOT NULL,
  `vch_areaname` varchar(12) DEFAULT NULL,
  PRIMARY KEY (`ch_areano`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_table_type`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_table_type`;
CREATE TABLE `cey_bt_table_type` (
  `ch_typeno` varchar(2) NOT NULL,
  `vch_typename` varchar(12) NOT NULL,
  `ch_pricekind` varchar(1) DEFAULT NULL,
  `ch_service_mode` varchar(1) DEFAULT NULL,
  `ch_serviceflag` varchar(1) DEFAULT NULL,
  `int_start_minute` int(11) DEFAULT NULL,
  `num_start_amount` decimal(12,2) DEFAULT NULL,
  `int_unit_minute` int(11) DEFAULT NULL,
  `num_unit_amount` decimal(12,2) DEFAULT NULL,
  `int_over` int(11) DEFAULT NULL,
  `num_ration` decimal(12,2) DEFAULT NULL,
  `int_rate` int(11) DEFAULT NULL,
  `num_max_service` decimal(12,2) DEFAULT NULL,
  `ch_low_mode` varchar(1) DEFAULT NULL,
  `num_lowcost` decimal(12,2) DEFAULT NULL,
  `int_begin_time` int(11) DEFAULT NULL,
  PRIMARY KEY (`ch_typeno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_bt_unit`
-- ----------------------------
DROP TABLE IF EXISTS `cey_bt_unit`;
CREATE TABLE `cey_bt_unit` (
  `ch_unitno` varchar(2) NOT NULL,
  `vch_unitname` varchar(10) NOT NULL,
  `ch_numtype` varchar(1) DEFAULT '0',
  `num_default` decimal(12,2) DEFAULT '1.00',
  PRIMARY KEY (`ch_unitno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_st_check_detail`
-- ----------------------------
DROP TABLE IF EXISTS `cey_st_check_detail`;
CREATE TABLE `cey_st_check_detail` (
  `ch_sheetno` varchar(12) NOT NULL,
  `ch_materialno` varchar(7) NOT NULL,
  `num_sys` decimal(18,4) DEFAULT '0.0000',
  `num_check` decimal(18,4) DEFAULT '0.0000',
  `num_pro_loss` decimal(18,4) DEFAULT '0.0000',
  `cost_price` decimal(18,4) DEFAULT '0.0000',
  `chg_flag` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`ch_materialno`,`ch_sheetno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_st_check_master`
-- ----------------------------
DROP TABLE IF EXISTS `cey_st_check_master`;
CREATE TABLE `cey_st_check_master` (
  `ch_sheetno` varchar(12) NOT NULL,
  `vch_handle` varchar(6) DEFAULT NULL,
  `vch_operId` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_audit` varchar(6) DEFAULT NULL,
  `dt_audit` datetime DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT NULL,
  `vch_memo` varchar(60) DEFAULT NULL,
  PRIMARY KEY (`ch_sheetno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_st_material_flow`
-- ----------------------------
DROP TABLE IF EXISTS `cey_st_material_flow`;
CREATE TABLE `cey_st_material_flow` (
  `int_flowId` int(11) NOT NULL AUTO_INCREMENT,
  `ch_materialno` varchar(7) NOT NULL,
  `dt_operdate` datetime NOT NULL,
  `sheettype` varchar(3) NOT NULL,
  `int_flag` int(11) NOT NULL,
  `num_num` decimal(18,4) DEFAULT '0.0000',
  `sheet_price` decimal(18,4) DEFAULT '0.0000',
  `cost_price` decimal(18,4) DEFAULT '0.0000',
  `chg_amount` decimal(18,4) DEFAULT '0.0000',
  `remain_qty` decimal(18,4) DEFAULT '0.0000',
  `remain_price` decimal(18,4) DEFAULT '0.0000',
  `ch_sheetno` varchar(12) DEFAULT NULL,
  PRIMARY KEY (`int_flowId`),
  KEY `Index_1` (`ch_materialno`,`dt_operdate`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_st_material_num`
-- ----------------------------
DROP TABLE IF EXISTS `cey_st_material_num`;
CREATE TABLE `cey_st_material_num` (
  `ch_materialno` varchar(7) NOT NULL,
  `num_num` decimal(18,4) DEFAULT '0.0000',
  `cost_price` decimal(18,4) DEFAULT '0.0000',
  PRIMARY KEY (`ch_materialno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_st_sheet_detail`
-- ----------------------------
DROP TABLE IF EXISTS `cey_st_sheet_detail`;
CREATE TABLE `cey_st_sheet_detail` (
  `ch_sheetno` varchar(12) NOT NULL,
  `int_id` int(11) NOT NULL,
  `ch_materialno` varchar(7) DEFAULT NULL,
  `num_num` decimal(18,4) DEFAULT '0.0000',
  `num_wrapping` decimal(18,4) DEFAULT '0.0000',
  `num_present` decimal(17,4) DEFAULT '0.0000',
  `num_price` decimal(18,4) DEFAULT '0.0000',
  `cost_price` decimal(18,4) DEFAULT '0.0000',
  `num_already` decimal(18,4) DEFAULT '0.0000',
  `vch_memo` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`ch_sheetno`,`int_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_st_sheet_master`
-- ----------------------------
DROP TABLE IF EXISTS `cey_st_sheet_master`;
CREATE TABLE `cey_st_sheet_master` (
  `ch_sheetNo` varchar(12) NOT NULL,
  `ch_sheetType` varchar(2) NOT NULL,
  `ch_operation` varchar(3) DEFAULT NULL,
  `ch_providerNo` varchar(8) DEFAULT NULL,
  `vch_handle` varchar(6) DEFAULT NULL,
  `vch_memo` varchar(60) DEFAULT NULL,
  `vch_operId` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_audit` varchar(6) DEFAULT NULL,
  `dt_audit` datetime DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`ch_sheetNo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_sys_company`
-- ----------------------------
DROP TABLE IF EXISTS `cey_sys_company`;
CREATE TABLE `cey_sys_company` (
  `vch_company` varchar(50) NOT NULL,
  `vch_principal` varchar(16) DEFAULT NULL,
  `vch_tel` varchar(16) DEFAULT NULL,
  `vch_handtel` varchar(16) DEFAULT NULL,
  `vch_email` varchar(40) DEFAULT NULL,
  `vch_address` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`vch_company`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_sys_operator`
-- ----------------------------
DROP TABLE IF EXISTS `cey_sys_operator`;
CREATE TABLE `cey_sys_operator` (
  `vch_operID` varchar(6) NOT NULL,
  `vch_operator` varchar(16) DEFAULT NULL,
  `vch_password` varchar(10) DEFAULT NULL,
  `ch_flag` varchar(1) DEFAULT 'N',
  `int_discount` int(11) DEFAULT '100',
  `ch_pre_mode` varchar(1) DEFAULT '1',
  `num_present` decimal(12,2) DEFAULT '0.00',
  `num_free` decimal(12,2) DEFAULT '0.00',
  `ch_waiter` varchar(1) DEFAULT 'N',
  `ch_free_flag` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`vch_operID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_sys_oper_purview`
-- ----------------------------
DROP TABLE IF EXISTS `cey_sys_oper_purview`;
CREATE TABLE `cey_sys_oper_purview` (
  `vch_operID` varchar(6) NOT NULL,
  `ch_purviewno` varchar(4) NOT NULL,
  PRIMARY KEY (`ch_purviewno`,`vch_operID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_sys_parameter`
-- ----------------------------
DROP TABLE IF EXISTS `cey_sys_parameter`;
CREATE TABLE `cey_sys_parameter` (
  `vch_parameter` varchar(30) NOT NULL,
  `vch_value` varchar(60) DEFAULT NULL,
  `vch_explain` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`vch_parameter`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_sys_purview_list`
-- ----------------------------
DROP TABLE IF EXISTS `cey_sys_purview_list`;
CREATE TABLE `cey_sys_purview_list` (
  `ch_purviewno` varchar(4) NOT NULL,
  `vch_purviewname` varchar(80) NOT NULL,
  `ch_day_cue` varchar(1) DEFAULT NULL,
  `ch_open_type` varchar(4) DEFAULT NULL,
  `vch_window` varchar(40) DEFAULT NULL,
  `vch_purview_typename` varchar(80) DEFAULT NULL,
  `ch_purview_typeno` varchar(2) DEFAULT NULL,
  `ch_report` varchar(1) DEFAULT 'N',
  `vch_withparm` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`ch_purviewno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_sys_sqlversion`
-- ----------------------------
DROP TABLE IF EXISTS `cey_sys_sqlversion`;
CREATE TABLE `cey_sys_sqlversion` (
  `sqlversion` varchar(255) NOT NULL,
  `sqlupdatetime` datetime DEFAULT NULL,
  PRIMARY KEY (`sqlversion`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_checkout_detail`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_checkout_detail`;
CREATE TABLE `cey_u_checkout_detail` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `ch_payno` varchar(15) NOT NULL,
  `ch_paymodeno` varchar(2) NOT NULL,
  `num_payamount` decimal(12,2) DEFAULT NULL,
  `num_change` decimal(12,2) DEFAULT '0.00',
  `num_realamount` decimal(12,2) DEFAULT '0.00',
  `num_face` decimal(12,2) DEFAULT '0.00',
  `vch_voucherno` varchar(30) DEFAULT NULL,
  `vch_memo` varchar(60) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `num_card_remain` decimal(12,2) DEFAULT '0.00',
  `num_face_Num` decimal(12,2) NOT NULL DEFAULT '0.00',
  PRIMARY KEY (`int_flowID`),
  KEY `idx_cey_u_checkout_detail_1` (`ch_payno`),
  KEY `idx_cey_u_checkout_detail_2` (`dt_operdate`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_checkout_master`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_checkout_master`;
CREATE TABLE `cey_u_checkout_master` (
  `ch_payno` varchar(15) NOT NULL,
  `ch_billno` varchar(15) NOT NULL,
  `ch_tableno` varchar(4) NOT NULL,
  `num_cost` decimal(12,2) DEFAULT NULL,
  `num_discount` decimal(12,2) DEFAULT NULL,
  `num_present` decimal(12,2) DEFAULT NULL,
  `num_service` decimal(12,2) DEFAULT NULL,
  `num_lowcost` decimal(12,2) DEFAULT NULL,
  `num_blotout` decimal(12,2) DEFAULT NULL,
  `num_free` decimal(12,2) DEFAULT NULL,
  `vch_free_operID` varchar(6) DEFAULT NULL,
  `num_cash` decimal(12,2) DEFAULT NULL,
  `num_other` decimal(12,2) DEFAULT NULL,
  `num_run` decimal(12,2) DEFAULT NULL,
  `vch_run_operID` varchar(6) DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_operID2` varchar(6) DEFAULT NULL,
  `dt_operdate2` datetime DEFAULT NULL,
  `vch_memberno` varchar(20) DEFAULT NULL,
  `ch_type` varchar(1) DEFAULT 'N',
  `num_rate` decimal(12,2) DEFAULT NULL,
  PRIMARY KEY (`ch_payno`),
  KEY `idx_cey_u_checkout_master_1` (`ch_billno`),
  KEY `idx_cey_u_checkout_master_2` (`ch_tableno`),
  KEY `idx_cey_u_checkout_master_3` (`dt_operdate`),
  KEY `idx_cey_u_checkout_master_4` (`ch_state`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_day_sumup`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_day_sumup`;
CREATE TABLE `cey_u_day_sumup` (
  `dt_date` datetime NOT NULL,
  `dt_begin` datetime NOT NULL,
  `dt_end` datetime NOT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `ch_upflag` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`dt_date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_handover_detail`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_handover_detail`;
CREATE TABLE `cey_u_handover_detail` (
  `int_id` int(11) NOT NULL,
  `vch_paymode` varchar(60) NOT NULL,
  `int_count` int(11) DEFAULT NULL,
  `num_amount` decimal(12,2) DEFAULT NULL,
  `num_face` decimal(12,2) DEFAULT NULL,
  PRIMARY KEY (`int_id`,`vch_paymode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_handover_master`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_handover_master`;
CREATE TABLE `cey_u_handover_master` (
  `int_id` int(11) NOT NULL,
  `vch_operID` varchar(20) NOT NULL,
  `dt_begin` datetime NOT NULL,
  `dt_end` datetime NOT NULL,
  `vch_accept` varchar(60) DEFAULT NULL,
  `vch_memo` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`int_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_handover_total`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_handover_total`;
CREATE TABLE `cey_u_handover_total` (
  `int_id` int(11) NOT NULL,
  `int_row` int(11) NOT NULL,
  `vch_item` varchar(100) NOT NULL,
  `num_amount` decimal(12,2) DEFAULT NULL,
  `vch_item2` varchar(100) DEFAULT NULL,
  `num_amount2` decimal(12,2) DEFAULT NULL,
  `int_height` int(11) DEFAULT '84',
  PRIMARY KEY (`int_id`,`int_row`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_hungbill`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_hungbill`;
CREATE TABLE `cey_u_hungbill` (
  `int_flowID` int(11) NOT NULL,
  `int_id` int(11) NOT NULL,
  `ch_tableno` varchar(4) DEFAULT NULL,
  `ch_dishno` varchar(7) DEFAULT NULL,
  `num_price` decimal(12,2) DEFAULT NULL,
  `num_num` decimal(12,2) DEFAULT NULL,
  `num_back` decimal(12,2) DEFAULT NULL,
  `vch_back_operID` varchar(6) DEFAULT NULL,
  `dt_back_operdate` datetime DEFAULT NULL,
  `int_discount` int(11) DEFAULT NULL,
  `vch_dis_operID` varchar(6) DEFAULT NULL,
  `dt_dis_operdate` datetime DEFAULT NULL,
  `ch_printflag` varchar(1) DEFAULT NULL,
  `vch_print_memo` varchar(50) DEFAULT NULL,
  `ch_suitflag` varchar(1) DEFAULT NULL,
  `ch_suitno` varchar(7) DEFAULT NULL,
  `ch_specialflag` varchar(1) DEFAULT NULL,
  `ch_presentflag` varchar(1) DEFAULT NULL,
  `vch_pre_operID` varchar(6) DEFAULT NULL,
  `dt_pre_operdate` datetime DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `num_price_add` decimal(20,2) DEFAULT '0.00',
  PRIMARY KEY (`int_flowID`,`int_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_master`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_master`;
CREATE TABLE `cey_u_master` (
  `ch_billno` varchar(15) NOT NULL,
  `ch_state` varchar(1) NOT NULL DEFAULT '1',
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_operID2` varchar(6) DEFAULT NULL,
  `dt_operdate2` datetime DEFAULT NULL,
  `vch_memo` varchar(60) DEFAULT NULL,
  PRIMARY KEY (`ch_billno`),
  KEY `idx_cey_u_master_1` (`ch_billno`),
  KEY `idx_cey_u_master_3` (`dt_operdate`),
  KEY `idx_cey_u_master_4` (`ch_state`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_orderdish`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_orderdish`;
CREATE TABLE `cey_u_orderdish` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `ch_billno` varchar(15) NOT NULL,
  `ch_tableno` varchar(4) NOT NULL,
  `int_id` int(11) NOT NULL,
  `ch_dishno` varchar(7) DEFAULT NULL,
  `num_price` decimal(12,2) DEFAULT '0.00',
  `num_price_org` decimal(12,2) DEFAULT '0.00',
  `num_price_add` decimal(12,2) DEFAULT '0.00',
  `num_num` decimal(12,2) DEFAULT '0.00',
  `num_back` decimal(12,2) DEFAULT '0.00',
  `vch_back_operID` varchar(6) DEFAULT NULL,
  `dt_back_operdate` datetime DEFAULT NULL,
  `int_discount` int(11) DEFAULT NULL,
  `vch_dis_operID` varchar(6) DEFAULT NULL,
  `dt_dis_operdate` datetime DEFAULT NULL,
  `vch_memberno` varchar(20) DEFAULT NULL,
  `ch_consult` varchar(1) DEFAULT NULL,
  `ch_printflag` varchar(1) DEFAULT NULL,
  `vch_print_memo` varchar(255) DEFAULT NULL,
  `ch_suitflag` varchar(1) DEFAULT NULL,
  `ch_suitno` varchar(7) DEFAULT NULL,
  `ch_specialflag` varchar(1) DEFAULT NULL,
  `ch_presentflag` varchar(1) DEFAULT NULL,
  `vch_pre_operID` varchar(6) DEFAULT NULL,
  `dt_pre_operdate` datetime DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `ch_togono` varchar(15) DEFAULT NULL,
  `ch_payno` varchar(15) DEFAULT NULL,
  `ch_outflag` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`int_flowID`),
  UNIQUE KEY `Idx_cey_u_orderdish` (`ch_billno`,`ch_tableno`,`int_id`),
  UNIQUE KEY `Idx_u_orderdish` (`ch_billno`,`ch_tableno`,`int_id`),
  KEY `idx_cey_u_orderdish_4` (`ch_dishno`),
  KEY `idx_cey_u_orderdish_5` (`dt_operdate`),
  KEY `idx_cey_u_orderdish_6` (`ch_payno`)
) ENGINE=InnoDB AUTO_INCREMENT=35 DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_orderdish_log`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_orderdish_log`;
CREATE TABLE `cey_u_orderdish_log` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `ch_type` varchar(4) NOT NULL,
  `vch_impower` varchar(6) DEFAULT NULL,
  `vch_reason` varchar(60) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_memberno` varchar(30) DEFAULT NULL,
  `vch_memo` varchar(40) DEFAULT NULL,
  `int_orderflow` int(11) DEFAULT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_service_low`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_service_low`;
CREATE TABLE `cey_u_service_low` (
  `ch_payno` varchar(15) NOT NULL,
  `ch_type` varchar(1) NOT NULL,
  `num_amount` decimal(12,2) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT NULL,
  `vch_memo` varchar(60) DEFAULT NULL,
  PRIMARY KEY (`ch_payno`,`ch_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_table`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_table`;
CREATE TABLE `cey_u_table` (
  `ch_billno` varchar(15) NOT NULL,
  `ch_tableno` varchar(4) NOT NULL,
  `int_person` int(11) DEFAULT NULL,
  `vch_memo` varchar(60) DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT '1',
  `ch_payno` varchar(15) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_operID2` varchar(6) DEFAULT NULL,
  `dt_operdate2` datetime DEFAULT NULL,
  `ch_order_state` varchar(1) DEFAULT '0',
  `num_cost` decimal(12,2) DEFAULT NULL,
  `vch_waiter` varchar(6) DEFAULT NULL,
  `num_trans_service` decimal(12,2) DEFAULT NULL,
  PRIMARY KEY (`ch_billno`),
  KEY `idx_cey_u_table_1` (`ch_billno`),
  KEY `idx_cey_u_table_2` (`ch_tableno`),
  KEY `idx_cey_u_table_3` (`dt_operdate`),
  KEY `idx_cey_u_table_4` (`ch_payno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_togo`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_togo`;
CREATE TABLE `cey_u_togo` (
  `ch_togono` varchar(15) NOT NULL,
  `ch_billno` varchar(15) NOT NULL,
  `ch_payno` varchar(15) DEFAULT NULL,
  `num_cost` decimal(12,2) DEFAULT NULL,
  `num_discount` decimal(12,2) DEFAULT NULL,
  `num_present` decimal(12,2) DEFAULT NULL,
  `num_blotout` decimal(12,2) DEFAULT NULL,
  `vch_waiter` varchar(6) DEFAULT NULL,
  `vch_address` varchar(50) DEFAULT NULL,
  `vch_linkman` varchar(20) DEFAULT NULL,
  `vch_tel` varchar(15) DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `num_rate` decimal(12,2) DEFAULT NULL,
  PRIMARY KEY (`ch_togono`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_togo_guest`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_togo_guest`;
CREATE TABLE `cey_u_togo_guest` (
  `ch_guestid` varchar(15) NOT NULL,
  `vch_guestname` varchar(20) NOT NULL,
  `vch_address` varchar(50) DEFAULT NULL,
  `vch_tel` varchar(15) DEFAULT NULL,
  PRIMARY KEY (`ch_guestid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_u_uncheckout_log`
-- ----------------------------
DROP TABLE IF EXISTS `cey_u_uncheckout_log`;
CREATE TABLE `cey_u_uncheckout_log` (
  `int_flowid` int(11) NOT NULL AUTO_INCREMENT,
  `ch_billno` varchar(15) DEFAULT NULL,
  `ch_payno_un` varchar(15) DEFAULT NULL,
  `num_cost_un` decimal(12,2) DEFAULT NULL,
  `vch_operid` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  PRIMARY KEY (`int_flowid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_web_order_dish`
-- ----------------------------
DROP TABLE IF EXISTS `cey_web_order_dish`;
CREATE TABLE `cey_web_order_dish` (
  `sn` float NOT NULL COMMENT '流水号',
  `orderSn` float DEFAULT NULL COMMENT '订单编号',
  `dishNo` varchar(255) DEFAULT NULL COMMENT '菜品编号',
  `dishName` varchar(255) DEFAULT NULL COMMENT '菜品名称',
  `number` float DEFAULT NULL COMMENT '数量',
  `price` float DEFAULT NULL COMMENT '价格',
  `suitFlag` decimal(10,0) DEFAULT NULL COMMENT '套菜标志位\r\n0：不是套菜\r\n1：是套菜',
  `note` varchar(255) DEFAULT NULL COMMENT '备注',
  `orgCode` varchar(255) DEFAULT NULL COMMENT 'orgCode',
  `date` datetime DEFAULT NULL COMMENT '插入表时间',
  `downFlag` decimal(10,0) DEFAULT NULL COMMENT '下发状态位\r\n0：未下发\r\n1：已经下发',
  PRIMARY KEY (`sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_web_order_info`
-- ----------------------------
DROP TABLE IF EXISTS `cey_web_order_info`;
CREATE TABLE `cey_web_order_info` (
  `sn` float NOT NULL COMMENT '订单编号',
  `eatTime` datetime DEFAULT NULL COMMENT '就餐时间',
  `eatPels` float DEFAULT NULL COMMENT '就餐人数',
  `eatTableNo` varchar(64) DEFAULT NULL COMMENT '餐桌编号',
  `tableAreaNo` varchar(64) DEFAULT NULL COMMENT '餐区编号',
  `tel` varchar(64) DEFAULT NULL COMMENT '手机号码',
  `orderer` varchar(64) DEFAULT NULL COMMENT '预订人',
  `sex` varchar(64) DEFAULT NULL COMMENT '性别',
  `note` varchar(64) DEFAULT NULL COMMENT '订单备注',
  `address` varchar(255) DEFAULT NULL COMMENT '地址',
  `sendTime` datetime DEFAULT NULL COMMENT '送达时间',
  `payType` varchar(64) DEFAULT NULL COMMENT '支付方式',
  `payState` varchar(64) DEFAULT NULL COMMENT '支付状态',
  `orderType` varchar(64) DEFAULT NULL COMMENT '订单类型',
  `orderState` varchar(64) DEFAULT NULL COMMENT '订单状态',
  `openid` varchar(128) DEFAULT NULL,
  `orgCode` varchar(64) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  `refuseReason` varchar(255) DEFAULT NULL COMMENT '订桌，外卖，拒绝理由',
  `downFlag` decimal(10,0) DEFAULT NULL COMMENT '下发状态位',
  PRIMARY KEY (`sn`),
  KEY `sn` (`sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cey_web_pay_for_water`
-- ----------------------------
DROP TABLE IF EXISTS `cey_web_pay_for_water`;
CREATE TABLE `cey_web_pay_for_water` (
  `sn` float NOT NULL COMMENT '支付编号',
  `orderSn` float NOT NULL COMMENT '订桌、外卖编号',
  `mount` float DEFAULT NULL COMMENT '金额',
  `note` varchar(255) DEFAULT NULL COMMENT '备注',
  `openid` varchar(128) DEFAULT NULL,
  `orgCode` varchar(64) DEFAULT NULL,
  `date` datetime DEFAULT NULL COMMENT '插入表时间',
  `upLoadFlag` decimal(10,0) DEFAULT NULL COMMENT '客户机上传状态位（服务器端可忽略）\r\n0：未上传\r\n1：已上传',
  PRIMARY KEY (`sn`),
  KEY `paySn` (`sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cybr_book_detail`
-- ----------------------------
DROP TABLE IF EXISTS `cybr_book_detail`;
CREATE TABLE `cybr_book_detail` (
  `ch_bookno` varchar(12) NOT NULL,
  `int_id` int(11) NOT NULL,
  `ch_dishno` varchar(7) NOT NULL,
  `vch_spec` varchar(12) DEFAULT NULL,
  `num_num` decimal(14,2) DEFAULT NULL,
  `num_price` decimal(14,2) DEFAULT NULL,
  `vch_print_memo` varchar(50) DEFAULT NULL,
  `num_price_add` decimal(14,2) DEFAULT NULL,
  `ch_suitflag` varchar(1) DEFAULT NULL,
  `ch_suitno` varchar(7) DEFAULT NULL,
  `int_material_rate` int(11) DEFAULT NULL,
  `ch_fixprice` varchar(1) DEFAULT NULL,
  `ch_specialflag` varchar(1) DEFAULT NULL,
  `ch_eatflag` varchar(1) NOT NULL DEFAULT 'N',
  PRIMARY KEY (`ch_bookno`,`int_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cybr_book_master`
-- ----------------------------
DROP TABLE IF EXISTS `cybr_book_master`;
CREATE TABLE `cybr_book_master` (
  `ch_bookno` varchar(12) NOT NULL,
  `ch_type` varchar(1) NOT NULL,
  `dt_come` datetime DEFAULT NULL,
  `int_overtime` int(11) DEFAULT NULL,
  `dt_warn` datetime DEFAULT NULL,
  `int_person` int(11) DEFAULT NULL,
  `vch_tel` varchar(20) DEFAULT NULL,
  `vch_booker` varchar(20) DEFAULT NULL,
  `ch_booker_type` varchar(1) DEFAULT NULL,
  `vch_ID` varchar(20) DEFAULT NULL,
  `vch_tel_second` varchar(20) DEFAULT NULL,
  `vch_address` varchar(50) DEFAULT NULL,
  `vch_memo` varchar(50) DEFAULT NULL,
  `num_deposit` decimal(14,2) DEFAULT NULL,
  `num_table_price` decimal(14,2) DEFAULT NULL,
  `ch_pricekind` varchar(1) DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT NULL,
  `dt_come_real` datetime DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `ch_sex` varchar(20) DEFAULT NULL,
  `ch_suitno` varchar(7) DEFAULT NULL,
  `vch_markid` varchar(6) DEFAULT NULL,
  `vch_tables` varchar(100) DEFAULT NULL,
  `ch_upflag` varchar(1) DEFAULT 'N',
  `ch_bustype` varchar(2) DEFAULT NULL,
  `suit_num` decimal(14,2) DEFAULT '0.00',
  `vch_liking` varchar(50) DEFAULT NULL,
  `vch_memo1` varchar(50) DEFAULT NULL,
  `vch_memo2` varchar(50) DEFAULT NULL,
  `vch_memo3` varchar(50) DEFAULT NULL,
  `dt_come_lunar` datetime DEFAULT NULL,
  `vch_character` varchar(40) DEFAULT NULL,
  `int_seat` int(11) DEFAULT NULL,
  `ch_mode` varchar(1) DEFAULT '0',
  `vch_fax` varchar(40) DEFAULT NULL,
  `ch_interno` varchar(15) DEFAULT NULL,
  PRIMARY KEY (`ch_bookno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `cybr_book_table`
-- ----------------------------
DROP TABLE IF EXISTS `cybr_book_table`;
CREATE TABLE `cybr_book_table` (
  `ch_bookno` varchar(12) NOT NULL,
  `ch_tableno` varchar(4) NOT NULL,
  `ch_open_flag` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`ch_bookno`,`ch_tableno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_mess_flow`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_mess_flow`;
CREATE TABLE `mess_t_mess_flow` (
  `flow_id` decimal(20,0) NOT NULL,
  `cust_no` varchar(20) DEFAULT NULL,
  `group_no` varchar(4) DEFAULT NULL,
  `mobile` varchar(20) DEFAULT NULL,
  `oper_id` varchar(6) DEFAULT NULL,
  `oper_date` datetime DEFAULT NULL,
  `send_date` datetime DEFAULT NULL,
  `mess` varchar(200) DEFAULT NULL,
  `region` varchar(1) DEFAULT NULL,
  `mess_id` decimal(20,0) DEFAULT NULL,
  `memo` varchar(100) DEFAULT NULL,
  `cust_name` varchar(20) DEFAULT NULL,
  `plan_id` decimal(20,0) DEFAULT NULL,
  `inout_type` varchar(1) DEFAULT NULL,
  `branch_no` varchar(10) DEFAULT NULL,
  `error` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`flow_id`),
  KEY `i_bd_supcust_item_1` (`group_no`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_set_plan`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_set_plan`;
CREATE TABLE `mess_t_set_plan` (
  `flow_id` int(11) NOT NULL AUTO_INCREMENT,
  `plan_type` varchar(1) DEFAULT NULL,
  `group_no` varchar(11) DEFAULT NULL,
  `group_name` varchar(30) DEFAULT NULL,
  `region` varchar(1) DEFAULT NULL,
  `date_type` varchar(2) DEFAULT NULL,
  `send_date` varchar(10) DEFAULT NULL,
  `send_time` datetime DEFAULT NULL,
  `plan_desc` varchar(20) DEFAULT NULL,
  `send_mess` varchar(140) DEFAULT NULL,
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_sys_system`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_sys_system`;
CREATE TABLE `mess_t_sys_system` (
  `sys_var_id` varchar(20) NOT NULL,
  `sys_var_name` varchar(40) DEFAULT NULL,
  `sys_var_value` varchar(140) DEFAULT NULL,
  PRIMARY KEY (`sys_var_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_vip_accnum`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_vip_accnum`;
CREATE TABLE `mess_t_vip_accnum` (
  `flow_id` decimal(20,0) NOT NULL,
  `card_id` varchar(20) NOT NULL,
  `acc_num` decimal(18,4) DEFAULT NULL,
  `send_flag` varchar(1) DEFAULT '0',
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_vip_birthday`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_vip_birthday`;
CREATE TABLE `mess_t_vip_birthday` (
  `flow_id` decimal(20,0) NOT NULL,
  `card_id` varchar(20) NOT NULL,
  `send_year` varchar(4) NOT NULL,
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_vip_boss`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_vip_boss`;
CREATE TABLE `mess_t_vip_boss` (
  `flow_id` decimal(20,0) NOT NULL,
  `oper_date` varchar(10) NOT NULL,
  `ope_date` datetime NOT NULL,
  `card_id` varchar(20) NOT NULL,
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_vip_card_little`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_vip_card_little`;
CREATE TABLE `mess_t_vip_card_little` (
  `flow_id` decimal(20,0) NOT NULL,
  `ope_date` datetime NOT NULL,
  `card_id` varchar(20) NOT NULL,
  `card_little` varchar(10) NOT NULL,
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_vip_flow`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_vip_flow`;
CREATE TABLE `mess_t_vip_flow` (
  `flow_id` decimal(20,0) NOT NULL,
  `card_id` varchar(20) NOT NULL,
  `acc_num` decimal(18,4) DEFAULT NULL,
  `flow_amt` decimal(18,4) DEFAULT NULL,
  `oper_type` varchar(1) DEFAULT NULL,
  `send_flag` varchar(1) DEFAULT '0',
  `flow_no` varchar(20) DEFAULT NULL,
  `item_no` varchar(20) DEFAULT NULL,
  `oper_date` datetime DEFAULT NULL,
  `send_flag1` varchar(1) DEFAULT '0',
  `send_time` varchar(2) DEFAULT NULL,
  `consumer_flag` varchar(1) DEFAULT '0',
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_vip_giv`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_vip_giv`;
CREATE TABLE `mess_t_vip_giv` (
  `flow_id` decimal(20,0) NOT NULL,
  `card_id` varchar(20) NOT NULL,
  `father_card` varchar(20) NOT NULL,
  `giv_acc_num` decimal(18,4) DEFAULT NULL,
  `oper_type` varchar(1) DEFAULT NULL,
  `change_amt` decimal(18,4) DEFAULT '0.0000',
  `send_flag` varchar(1) DEFAULT '0',
  `oper_date` datetime DEFAULT NULL,
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_vip_info`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_vip_info`;
CREATE TABLE `mess_t_vip_info` (
  `flow_id` decimal(20,0) NOT NULL,
  `card_id` varchar(20) NOT NULL,
  `father_card` varchar(20) DEFAULT NULL,
  `change_amt` decimal(18,4) DEFAULT '0.0000',
  `send_flag` varchar(1) DEFAULT '0',
  `oper_date` datetime DEFAULT NULL,
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_vip_message`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_vip_message`;
CREATE TABLE `mess_t_vip_message` (
  `flow_id` decimal(20,0) NOT NULL,
  `mess_type` varchar(10) NOT NULL,
  `send_flag` varchar(1) NOT NULL DEFAULT '0',
  `oper_date` datetime DEFAULT NULL,
  `branch_no` varchar(10) NOT NULL,
  `card_id` varchar(20) NOT NULL,
  `card_type` varchar(2) DEFAULT NULL,
  `to_card_type` varchar(2) DEFAULT NULL,
  `father_card` varchar(20) DEFAULT NULL,
  `giv_acc_num` decimal(18,4) DEFAULT NULL,
  `oper_type` varchar(1) DEFAULT NULL,
  `change_amt` decimal(18,4) DEFAULT '0.0000',
  `savelist_amt` decimal(18,4) DEFAULT NULL,
  `flow_no` varchar(20) DEFAULT NULL,
  `item_no` varchar(20) DEFAULT NULL,
  `acc_num` decimal(18,4) DEFAULT NULL,
  `flow_amt` decimal(18,4) DEFAULT NULL,
  `send_year` varchar(4) DEFAULT NULL,
  `send_date` varchar(10) DEFAULT NULL,
  `card_little` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_vip_savelist`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_vip_savelist`;
CREATE TABLE `mess_t_vip_savelist` (
  `flow_id` decimal(20,0) NOT NULL,
  `card_id` varchar(20) NOT NULL,
  `savelist_amt` decimal(18,4) DEFAULT NULL,
  `oper_type` varchar(1) DEFAULT NULL,
  `send_flag` varchar(1) DEFAULT '0',
  `oper_date` datetime DEFAULT NULL,
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `mess_t_vip_type`
-- ----------------------------
DROP TABLE IF EXISTS `mess_t_vip_type`;
CREATE TABLE `mess_t_vip_type` (
  `flow_id` decimal(20,0) NOT NULL,
  `card_id` varchar(20) NOT NULL,
  `card_type` varchar(2) NOT NULL,
  `to_card_type` varchar(2) NOT NULL,
  `send_flag` varchar(1) DEFAULT '0',
  `oper_date` datetime DEFAULT NULL,
  PRIMARY KEY (`flow_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `pbcatcol`
-- ----------------------------
DROP TABLE IF EXISTS `pbcatcol`;
CREATE TABLE `pbcatcol` (
  `pbc_tnam` varchar(30) DEFAULT NULL,
  `pbc_tid` int(11) DEFAULT NULL,
  `pbc_ownr` varchar(30) DEFAULT NULL,
  `pbc_cnam` varchar(30) DEFAULT NULL,
  `pbc_cid` smallint(6) DEFAULT NULL,
  `pbc_labl` varchar(254) DEFAULT NULL,
  `pbc_lpos` smallint(6) DEFAULT NULL,
  `pbc_hdr` varchar(254) DEFAULT NULL,
  `pbc_hpos` smallint(6) DEFAULT NULL,
  `pbc_jtfy` smallint(6) DEFAULT NULL,
  `pbc_mask` varchar(31) DEFAULT NULL,
  `pbc_case` smallint(6) DEFAULT NULL,
  `pbc_hght` smallint(6) DEFAULT NULL,
  `pbc_wdth` smallint(6) DEFAULT NULL,
  `pbc_ptrn` varchar(31) DEFAULT NULL,
  `pbc_bmap` varchar(1) DEFAULT NULL,
  `pbc_init` varchar(254) DEFAULT NULL,
  `pbc_cmnt` varchar(254) DEFAULT NULL,
  `pbc_edit` varchar(31) DEFAULT NULL,
  `pbc_tag` varchar(254) DEFAULT NULL,
  UNIQUE KEY `pbcatcol_idx` (`pbc_tnam`,`pbc_ownr`,`pbc_cnam`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `pbcatedt`
-- ----------------------------
DROP TABLE IF EXISTS `pbcatedt`;
CREATE TABLE `pbcatedt` (
  `pbe_name` varchar(30) NOT NULL,
  `pbe_edit` varchar(254) DEFAULT NULL,
  `pbe_type` smallint(6) NOT NULL,
  `pbe_cntr` int(11) DEFAULT NULL,
  `pbe_seqn` smallint(6) NOT NULL,
  `pbe_flag` int(11) DEFAULT NULL,
  `pbe_work` varchar(32) DEFAULT NULL,
  UNIQUE KEY `pbcatedt_idx` (`pbe_name`,`pbe_seqn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `pbcatfmt`
-- ----------------------------
DROP TABLE IF EXISTS `pbcatfmt`;
CREATE TABLE `pbcatfmt` (
  `pbf_name` varchar(30) NOT NULL,
  `pbf_frmt` varchar(254) NOT NULL,
  `pbf_type` smallint(6) NOT NULL,
  `pbf_cntr` int(11) DEFAULT NULL,
  UNIQUE KEY `pbcatfmt_idx` (`pbf_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `pbcattbl`
-- ----------------------------
DROP TABLE IF EXISTS `pbcattbl`;
CREATE TABLE `pbcattbl` (
  `pbt_tnam` varchar(30) DEFAULT NULL,
  `pbt_tid` int(11) DEFAULT NULL,
  `pbt_ownr` varchar(30) DEFAULT NULL,
  `pbd_fhgt` smallint(6) DEFAULT NULL,
  `pbd_fwgt` smallint(6) DEFAULT NULL,
  `pbd_fitl` varchar(1) DEFAULT NULL,
  `pbd_funl` varchar(1) DEFAULT NULL,
  `pbd_fchr` smallint(6) DEFAULT NULL,
  `pbd_fptc` smallint(6) DEFAULT NULL,
  `pbd_ffce` varchar(32) DEFAULT NULL,
  `pbh_fhgt` smallint(6) DEFAULT NULL,
  `pbh_fwgt` smallint(6) DEFAULT NULL,
  `pbh_fitl` varchar(1) DEFAULT NULL,
  `pbh_funl` varchar(1) DEFAULT NULL,
  `pbh_fchr` smallint(6) DEFAULT NULL,
  `pbh_fptc` smallint(6) DEFAULT NULL,
  `pbh_ffce` varchar(32) DEFAULT NULL,
  `pbl_fhgt` smallint(6) DEFAULT NULL,
  `pbl_fwgt` smallint(6) DEFAULT NULL,
  `pbl_fitl` varchar(1) DEFAULT NULL,
  `pbl_funl` varchar(1) DEFAULT NULL,
  `pbl_fchr` smallint(6) DEFAULT NULL,
  `pbl_fptc` smallint(6) DEFAULT NULL,
  `pbl_ffce` varchar(32) DEFAULT NULL,
  `pbt_cmnt` varchar(254) DEFAULT NULL,
  UNIQUE KEY `pbcattbl_idx` (`pbt_tnam`,`pbt_ownr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `pbcatvld`
-- ----------------------------
DROP TABLE IF EXISTS `pbcatvld`;
CREATE TABLE `pbcatvld` (
  `pbv_name` varchar(30) NOT NULL,
  `pbv_vald` varchar(254) NOT NULL,
  `pbv_type` smallint(6) NOT NULL,
  `pbv_cntr` int(11) DEFAULT NULL,
  `pbv_msg` varchar(254) DEFAULT NULL,
  UNIQUE KEY `pbcatvld_idx` (`pbv_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_ms_comm_used`
-- ----------------------------
DROP TABLE IF EXISTS `t_ms_comm_used`;
CREATE TABLE `t_ms_comm_used` (
  `id` int(11) NOT NULL,
  `mess_title` varchar(50) DEFAULT NULL,
  `mess_content` text,
  `mess_type` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_ms_cust_info`
-- ----------------------------
DROP TABLE IF EXISTS `t_ms_cust_info`;
CREATE TABLE `t_ms_cust_info` (
  `group_no` varchar(4) NOT NULL,
  `cust_no` varchar(20) NOT NULL,
  `cust_name` varchar(20) DEFAULT NULL,
  `mobile` varchar(24) DEFAULT NULL,
  `sex` varchar(2) DEFAULT NULL,
  `addr` varchar(50) DEFAULT NULL,
  `zip` varchar(10) DEFAULT NULL,
  `phone` varchar(20) DEFAULT NULL,
  `e_mail` varchar(40) DEFAULT NULL,
  `birthday` datetime DEFAULT NULL,
  `company` varchar(50) DEFAULT NULL,
  `duty` varchar(20) DEFAULT NULL,
  `region` varchar(20) DEFAULT NULL,
  `id_card` varchar(20) DEFAULT NULL,
  `cust_type` varchar(20) DEFAULT '0',
  `begin_date` datetime DEFAULT NULL,
  `end_date` datetime DEFAULT NULL,
  `memo` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`cust_no`),
  KEY `i_bd_supcust_item_1` (`cust_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_ms_group_info`
-- ----------------------------
DROP TABLE IF EXISTS `t_ms_group_info`;
CREATE TABLE `t_ms_group_info` (
  `group_no` varchar(4) NOT NULL,
  `group_name` varchar(20) DEFAULT NULL,
  `type_flag` varchar(2) DEFAULT '0',
  `memo` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`group_no`),
  KEY `i_bd_supcust_item_1` (`group_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_ms_mess_flow`
-- ----------------------------
DROP TABLE IF EXISTS `t_ms_mess_flow`;
CREATE TABLE `t_ms_mess_flow` (
  `flow_id` decimal(20,0) NOT NULL,
  `cust_no` varchar(20) DEFAULT NULL,
  `group_no` varchar(4) DEFAULT NULL,
  `mobile` varchar(20) DEFAULT NULL,
  `oper_id` varchar(6) DEFAULT NULL,
  `oper_date` datetime DEFAULT NULL,
  `send_date` datetime DEFAULT NULL,
  `mess` text,
  `region` varchar(1) DEFAULT NULL,
  `mess_id` decimal(20,0) DEFAULT NULL,
  `memo` varchar(100) DEFAULT NULL,
  `cust_name` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`flow_id`),
  KEY `i_bd_supcust_item_1` (`group_no`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_curamount`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_curamount`;
CREATE TABLE `t_m_curamount` (
  `vch_memberno` varchar(20) NOT NULL,
  `num_amount` decimal(12,2) DEFAULT NULL,
  `num_hangamount` decimal(12,2) DEFAULT NULL,
  `num_point` decimal(12,2) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `ch_branchno` varchar(4) DEFAULT NULL,
  PRIMARY KEY (`vch_memberno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_deposit`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_deposit`;
CREATE TABLE `t_m_deposit` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `vch_memberno` varchar(20) NOT NULL,
  `num_deposit` decimal(12,2) DEFAULT NULL,
  `num_realamount` decimal(12,2) DEFAULT NULL,
  `ch_deposit_mode` varchar(1) DEFAULT NULL,
  `ch_pay_mode` varchar(1) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_memo` varchar(60) DEFAULT NULL,
  `remain_amt` decimal(17,2) DEFAULT '0.00',
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_gift`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_gift`;
CREATE TABLE `t_m_gift` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `ch_giftno` varchar(4) NOT NULL,
  `vch_giftname` varchar(30) NOT NULL,
  `num_point` decimal(12,2) DEFAULT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_hang_balance`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_hang_balance`;
CREATE TABLE `t_m_hang_balance` (
  `ch_balanceno` varchar(12) NOT NULL,
  `vch_memberno` varchar(20) NOT NULL,
  `dt_date` datetime DEFAULT NULL,
  `num_remain` decimal(20,2) DEFAULT NULL,
  `num_pay` decimal(20,2) DEFAULT NULL,
  `num_amount` decimal(20,2) DEFAULT NULL,
  `vch_balance` varchar(16) DEFAULT NULL,
  `vch_operId` varchar(16) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_memo` varchar(60) DEFAULT NULL,
  `ch_state` varchar(1) NOT NULL,
  `ch_type` varchar(1) NOT NULL,
  `ch_paymodeno` varchar(2) DEFAULT NULL,
  PRIMARY KEY (`ch_balanceno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_member`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_member`;
CREATE TABLE `t_m_member` (
  `vch_memberno` varchar(20) NOT NULL,
  `vch_member` varchar(40) NOT NULL,
  `ch_typeno` varchar(2) DEFAULT NULL,
  `vch_id` varchar(20) DEFAULT NULL,
  `dt_birthday` datetime DEFAULT NULL,
  `vch_tel` varchar(20) DEFAULT NULL,
  `vch_handtel` varchar(20) DEFAULT NULL,
  `vch_address` varchar(50) DEFAULT NULL,
  `dt_limit` datetime DEFAULT NULL,
  `ch_state` varchar(1) DEFAULT '1',
  `num_limit` decimal(12,2) DEFAULT NULL,
  `ch_cardflag` varchar(1) DEFAULT 'N',
  `vch_cardno` varchar(20) DEFAULT NULL,
  `vch_password` varchar(20) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `int_basedata` int(11) DEFAULT '0',
  `dt_sendtime` datetime DEFAULT NULL,
  PRIMARY KEY (`vch_memberno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_member_log`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_member_log`;
CREATE TABLE `t_m_member_log` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `vch_memberno` varchar(20) NOT NULL,
  `vch_matter` varchar(20) NOT NULL,
  `vch_memo` varchar(50) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `num_bace` decimal(20,2) DEFAULT '0.00',
  `dt_send_date` datetime DEFAULT NULL,
  `vch_member` varchar(40) DEFAULT NULL,
  `ch_typeno` varchar(2) DEFAULT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_member_other`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_member_other`;
CREATE TABLE `t_m_member_other` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `vch_memberno` varchar(20) NOT NULL,
  `vch_item` varchar(20) NOT NULL,
  `vch_memo` varchar(50) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_member_type`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_member_type`;
CREATE TABLE `t_m_member_type` (
  `ch_typeno` varchar(2) NOT NULL,
  `vch_typename` varchar(16) NOT NULL,
  `ch_attribute` varchar(1) NOT NULL,
  `ch_favourable` varchar(1) DEFAULT NULL,
  `ch_pricekind` varchar(1) DEFAULT NULL,
  `int_discount` int(11) DEFAULT NULL,
  `ch_projectno` varchar(3) DEFAULT NULL,
  `ch_calc` varchar(1) DEFAULT NULL,
  `ch_calc_type` varchar(1) DEFAULT NULL,
  `int_calc_unit` int(11) DEFAULT NULL,
  `ch_area_flag` varchar(1) DEFAULT NULL,
  `int_backcash_rate` int(11) DEFAULT NULL,
  `int_card_baseamount` int(11) DEFAULT NULL,
  PRIMARY KEY (`ch_typeno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_parameter`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_parameter`;
CREATE TABLE `t_m_parameter` (
  `vch_parameter` varchar(20) NOT NULL,
  `vch_value` varchar(30) DEFAULT NULL,
  `vch_explain` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`vch_parameter`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_pay`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_pay`;
CREATE TABLE `t_m_pay` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `vch_memberno` varchar(20) NOT NULL,
  `ch_payno` varchar(15) DEFAULT NULL,
  `ch_paymodeno` varchar(2) DEFAULT NULL,
  `num_amount` decimal(12,2) DEFAULT NULL,
  `num_pay` decimal(12,2) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `ch_balanceno` varchar(12) DEFAULT NULL,
  `int_checkflow` int(11) DEFAULT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_point`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_point`;
CREATE TABLE `t_m_point` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `vch_memberno` varchar(20) NOT NULL,
  `ch_type` varchar(1) NOT NULL,
  `ch_payno` varchar(15) DEFAULT NULL,
  `num_point` decimal(12,2) DEFAULT NULL,
  `vch_memo` varchar(50) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_point_gift`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_point_gift`;
CREATE TABLE `t_m_point_gift` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `vch_memberno` varchar(20) NOT NULL,
  `ch_giftname` varchar(30) DEFAULT NULL,
  `vch_operID` varchar(6) DEFAULT NULL,
  `num_point` decimal(12,2) DEFAULT NULL,
  `vch_memo` varchar(60) DEFAULT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `ch_branchno` varchar(4) DEFAULT NULL,
  `ch_upflag` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_m_rw_password_log`
-- ----------------------------
DROP TABLE IF EXISTS `t_m_rw_password_log`;
CREATE TABLE `t_m_rw_password_log` (
  `int_flowID` int(11) NOT NULL AUTO_INCREMENT,
  `vch_operID` varchar(6) NOT NULL,
  `dt_operdate` datetime DEFAULT NULL,
  `vch_password_old` varchar(10) DEFAULT NULL,
  `vch_password_new` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`int_flowID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_st_material_templet`
-- ----------------------------
DROP TABLE IF EXISTS `t_st_material_templet`;
CREATE TABLE `t_st_material_templet` (
  `ch_templetname` varchar(30) NOT NULL,
  `ch_materialno` varchar(7) NOT NULL,
  PRIMARY KEY (`ch_materialno`,`ch_templetname`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `t_sys_system`
-- ----------------------------
DROP TABLE IF EXISTS `t_sys_system`;
CREATE TABLE `t_sys_system` (
  `sys_var_id` varchar(20) NOT NULL,
  `sys_var_name` varchar(40) NOT NULL,
  `sys_var_value` varchar(80) DEFAULT NULL,
  `is_changed` varchar(2) DEFAULT NULL,
  `sys_var_desc` varchar(100) DEFAULT NULL,
  `sys_ver_flag` varchar(1) DEFAULT NULL,
  `sys_inited` varchar(1) DEFAULT NULL,
  `memo` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`sys_var_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Procedure definition for `get_time`
-- ----------------------------
DROP PROCEDURE IF EXISTS `get_time`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `get_time`(str1 CHAR(25), 
str2 CHAR(25),
temp1 CHAR(25),
temp2 CHAR(25))
BEGIN 

declare beginTime datetime;
declare endTime datetime;

	IF (select count(dt_begin) from cey_u_day_sumup where dt_date =str1 limit 1)>0 THEN
		set beginTime = (select dt_begin from cey_u_day_sumup where dt_date =str1 limit 1);
	ELSE 
		IF (select count(1) from cey_u_day_sumup where dt_date <= str1 )>0 THEN
	 		set beginTime = (select  IFNULL( MAX(dt_end),temp1) from cey_u_day_sumup where dt_date <= str1);
	  ELSE 
	  	set beginTime = (select  IFNULL( MIN(dt_begin),temp1) from cey_u_day_sumup where dt_date >= str1); 
		END IF; 
	END IF;


	IF (select count(dt_end) from cey_u_day_sumup where dt_date =str2 limit 1)>0 THEN
		set endTime = (select dt_end from cey_u_day_sumup where dt_date =str2 limit 1);
	ELSE 
		IF (select count( 1 ) from cey_u_day_sumup where dt_date <= str2)>0 THEN
	 		set endTime = (select IFNULL(MAX(dt_end),temp2) from cey_u_day_sumup where dt_date <= str2 ); 
		ELSE 
 	 		set endTime = (select IFNULL(MIN(dt_begin),temp2) from cey_u_day_sumup where dt_date >= str2); 
		END IF;
	END IF; 

 SELECT beginTime,endTime;

END
;;
DELIMITER ;

-- ----------------------------
--  Procedure definition for `get_time2`
-- ----------------------------
DROP PROCEDURE IF EXISTS `get_time2`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `get_time2`(str1 VARCHAR(25),str2 VARCHAR(25))
BEGIN
declare beginTime datetime ;
declare endTime datetime;
set beginTime=(Select dt_begin from cey_u_day_sumup where dt_date =str1);
set endTime=(Select dt_end from cey_u_day_sumup where dt_date =str2);
SELECT beginTime,endTime;
END
;;
DELIMITER ;

-- ----------------------------
--  Procedure definition for `get_time3`
-- ----------------------------
DROP PROCEDURE IF EXISTS `get_time3`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `get_time3`(str1 VARCHAR(25),str2 VARCHAR(25))
BEGIN
declare beginTime datetime ;
declare endTime datetime;
set beginTime=(Select dt_begin from cey_u_day_sumup where dt_date =str1);
set endTime=(Select dt_end from cey_u_day_sumup where dt_date =str2);
SELECT beginTime,endTime;
END
;;
DELIMITER ;

-- ----------------------------
--  Procedure definition for `pr_m_deposit_point_detail`
-- ----------------------------
DROP PROCEDURE IF EXISTS `pr_m_deposit_point_detail`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `pr_m_deposit_point_detail`(sdate VARCHAR(12),edate VARCHAR(12),memberno VARCHAR(12))
BEGIN

DROP TABLE IF EXISTS temp;

CREATE TEMPORARY TABLE temp(memberno varchar(20) not null,membername varchar(40) null,typeno varchar(30) null,		
								smoney decimal(18,2) default 0,spoint decimal(18,2) default 0,mpay decimal(18,2) default 0,
							 	mdeposit	decimal(18,2) default 0,	mpoint 	  decimal(18,2) default 0, emoney decimal(18,2) default 0,
								epoint decimal(18,2) default 0,dpdate datetime null,		
								memo varchar(100) null);
	
INSERT INTO temp
	SELECT A.vch_memberno memberno,B.vch_member vch_member,(select vch_typename from t_m_member_type where ch_typeno = B.ch_typeno limit 0,1) typeno, 
	  IFNULL((select sum(IFNULL(num_deposit,0)) from t_m_deposit where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) -
    IFNULL((select sum(IFNULL(num_pay,0)) from t_m_pay where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) + 
    IFNULL(num_limit,0) smoney,
	  IFNULL((select sum(IFNULL(num_point,0)) from t_m_point where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) spoint,
	  0 mpay,	IFNULL(num_deposit,0) mdeposit ,0 mpoint,
	  IFNULL((select sum(IFNULL(num_deposit,0)) from t_m_deposit where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) -
    IFNULL((select sum(IFNULL(num_pay,0)) from t_m_pay where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) + IFNULL(num_deposit,0) + IFNULL(num_limit,0) emoney,
 	  IFNULL((select sum(IFNULL(num_point,0)) from t_m_point where vch_memberno = A.vch_memberno and dt_operdate <= A.dt_operdate ),0) epoint,
    A.dt_operdate dpdate, case when A.ch_deposit_mode = '3' and ch_pay_mode = '5' then  concat('系统付款返现' , IFNULL(A.vch_memo,'')) else A.vch_memo end memo
  FROM t_m_deposit A, t_m_member B WHERE A.vch_memberno = B.vch_memberno AND LEFT(A.dt_operdate,10) >= sdate AND LEFT(A.dt_operdate,10) <= edate AND A.vch_memberno = memberno;

INSERT INTO temp
	SELECT A.vch_memberno memberno,B.vch_member vch_member,(select vch_typename from t_m_member_type where ch_typeno = B.ch_typeno LIMIT 0,1) typeno, 
	  IFNULL((select sum(IFNULL(num_deposit,0)) from t_m_deposit where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) -
    IFNULL((select sum(IFNULL(num_pay,0)) from t_m_pay where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) + 
    IFNULL(num_limit,0)  smoney,
	  IFNULL((select sum(IFNULL(num_point,0)) from t_m_point where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) spoint,
	  0 mpay,	0 mdeposit ,IFNULL( num_point,0) mpoint,
	  IFNULL((select sum(ifnull(num_deposit,0)) from t_m_deposit where vch_memberno = A.vch_memberno and dt_operdate <= A.dt_operdate ),0) -
    IFNULL((select sum(ifnull(num_pay,0)) from t_m_pay where vch_memberno = A.vch_memberno and dt_operdate <= A.dt_operdate ),0) + IFNULL(num_limit,0) emoney,
 	  IFNULL((select sum(ifnull(num_point,0)) from t_m_point where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) + IFNULL(num_point,0)  epoint,A.dt_operdate dpdate, case when A.ch_type = '3' then   concat('消费系统积分' , IFNULL(A.vch_memo,'')) else A.vch_memo end memo
  FROM t_m_point A, t_m_member B WHERE A.vch_memberno = B.vch_memberno AND IFNULL( A.num_point,0) <> 0	AND  LEFT(A.dt_operdate,10) >= sdate AND LEFT(A.dt_operdate,10) <= edate AND A.vch_memberno = memberno;
	
INSERT INTO temp
	 SELECT A.vch_memberno memberno,B.vch_member vch_member,(select vch_typename from t_m_member_type where ch_typeno = B.ch_typeno LIMIT 0,1) typeno, 
	   IFNULL((select sum(IFNULL(num_deposit,0)) from t_m_deposit where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) -
     IFNULL((select sum(IFNULL(num_pay,0)) from t_m_pay where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) + IFNULL(num_limit,0) smoney, 
	   IFNULL((select sum(IFNULL(num_point,0)) from t_m_point where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) spoint,
	   IFNULL(num_pay,0) mpay,0 mdeposit ,0 mpoint,
	   IFNULL((select sum(IFNULL(num_deposit,0)) from t_m_deposit where vch_memberno = A.vch_memberno and dt_operdate <= A.dt_operdate ),0) -
     IFNULL((select sum(IFNULL(num_pay,0)) from t_m_pay where vch_memberno = A.vch_memberno and dt_operdate < A.dt_operdate ),0) - IFNULL(num_pay,0) + IFNULL(num_limit,0) emoney,
 	   IFNULL((select sum(IFNULL( num_point,0)) from t_m_point where vch_memberno = A.vch_memberno and dt_operdate <= A.dt_operdate ),0) epoint,
     A.dt_operdate dpdate, '' memo
	 FROM t_m_pay A, t_m_member B WHERE A.vch_memberno = B.vch_memberno AND LEFT(A.dt_operdate,10) >= sdate AND LEFT(A.dt_operdate,10) <= edate AND A.vch_memberno = memberno;

	SELECT * FROM temp ORDER BY dpdate;

END
;;
DELIMITER ;

-- ----------------------------
--  Procedure definition for `sys_settings`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sys_settings`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sys_settings`(attribute VARCHAR(20000))
BEGIN
 DECLARE ix int;
 DECLARE pos int;
 DECLARE attributeLenght int;
 DECLARE str1 varchar(30);
 DECLARE str2 varchar(60);
 DECLARE str3 varchar(30);
	SET pos = 1 ;
  SET ix = 1 ;
  SET attributeLenght = LENGTH(attribute);




/*
重字符串中，分割数据。。。。。
每3个 一组。
*/

loop_label:WHILE ix>0 do

	SET ix = LOCATE('&,&', attribute, pos);
	SET str1 = SUBSTRING(attribute, pos, ix - pos);
  SET pos = ix + 3 ;


  SET ix = LOCATE('&,&', attribute, pos); 
	SET str2 = SUBSTRING(attribute, pos, ix - pos);
  SET pos = ix + 3 ;


  SET ix = LOCATE('&,&', attribute, pos); 
	IF ix > 0 THEN
		SET str3 = SUBSTRING(attribute, pos, ix - pos);
	ELSE
		SET str3 = substring(attribute, pos, attributeLenght);
	END IF;
	SET pos = ix + 3;


/*
执行 更新或者 插入操作。。。
*/

	IF (Select count(*) from cey_sys_parameter where vch_parameter =str1)>0 THEN 
		update cey_sys_parameter set vch_value =str2 where vch_parameter =str1;
	ELSE 
		Insert Into cey_sys_parameter values ( str1 , str2, str3);
	END IF;

SET ix = LOCATE('&,&', attribute, pos);


END WHILE;

END
;;
DELIMITER ;

-- ----------------------------
--  Procedure definition for `sys_set_permissions`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sys_set_permissions`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sys_set_permissions`(id VARCHAR(1000),list VARCHAR(1000))
BEGIN
 DECLARE ix int;
 DECLARE pos int;
 DECLARE str varchar(1000);
	SET pos = 1 ;
  SET ix = 1 ;

delete from cey_sys_oper_purview where vch_operID = id ;
WHILE ix > 0 do

	SET ix = LOCATE(',', list, pos); 
	IF ix > 0 THEN
		SET str = SUBSTRING(list, pos, ix - pos);
	ELSE 
		SET str = substring(list, pos, LENGTH(list));
	end if;
	insert into cey_sys_oper_purview values(id,str) ;
	SET pos = ix + 1 ;
END WHILE;
END
;;
DELIMITER ;

-- ----------------------------
--  Records 
-- ----------------------------
INSERT INTO `cey_bt_kitchen_print` VALUES ('1','xx','123',NULL,NULL);
INSERT INTO `cey_bt_paymode` VALUES ('00','现金','N','N','Y','Y','A'), ('02','充值卡','Y','N','N','Y','C'), ('03','银行卡','Y','N','N','Y','Y'), ('04','代金券','Y','N','N','Y','E'), ('05','微信支付','N','N','N','Y','W'), ('XX','免单金额','N','N','N','N',' '), ('YY','自动抹零','N','N','N','N',' '), ('ZZ','手工抹零','N','N','N','N',' ');
INSERT INTO `cey_sys_company` VALUES ('********酒楼',NULL,NULL,NULL,NULL,NULL);
INSERT INTO `cey_sys_parameter` VALUES ('blotout_mode','1','抹零方式'), ('blotout_round','1','抹零(1)或四舍五入(2)'), ('bolt_operator','1','抹零默认当前操作员'), ('bp_address','0','外卖地址的账单打印'), ('bp_address_len','12','账单打印外卖地址宽度'), ('bp_address_row','1','账单打印外卖地址行数'), ('bp_amount','1','是否打印金额'), ('bp_amount_dec','2','金额小位位'), ('bp_amount_len','9','金额列宽'), ('bp_amount_row','2','金额行数'), ('bp_avg','0','人均消费'), ('bp_blotout','1','抹零金额是否打印'), ('bp_cash','0','实收现金'), ('bp_cost','1','消费金额是否打印'), ('bp_count','1','账单连续打印份数'), ('bp_discount','1','折扣金额是否打印'), ('bp_discountproject','0','会员折扣'), ('bp_disdetail','0','是否打印折扣'), ('bp_disdetail_len','6','折扣列宽'), ('bp_disdetail_row','1','折扣行数'), ('bp_dishname','1','是否打印品名'), ('bp_dishname_len','18','品名列宽'), ('bp_dishname_row','1','品名行数'), ('bp_free','0','免单金额是否打印'), ('bp_identifier','1','是否打印账单标识'), ('bp_linkman','0','外卖联系电话的账单打印'), ('bp_linkman_len','12','账单打印外卖联系电话宽度'), ('bp_linkman_row','1','账单打印外卖联系电话行数'), ('bp_lowcost','0','低消金额是否打印'), ('bp_membername','0','会员名称'), ('bp_membertype','0','会员类型'), ('bp_memo','0','开台备注'), ('bp_memo_len','12','开台备注打印宽度'), ('bp_memo_row','1','开台备注打印行数'), ('bp_num','1','是否打印数量'), ('bp_num_dec','2','数量小位位'), ('bp_num_len','9','数量列宽'), ('bp_num_row','2','数量行数'), ('bp_offcols','2','表尾每行打印信息数'), ('bp_operid','1','工号的账单打印'), ('bp_operid_len','12','账单打印工号宽度'), ('bp_operid_row','2','账单打印工号行数'), ('bp_overstep_cut','1','超出字符设置截取'), ('bp_payno','1','付款单号的账单打印'), ('bp_payno_len','16','账单打印单号宽度'), ('bp_payno_row','1','账单打印单号行数'), ('bp_person','0','人数的账单打印'), ('bp_person_len','12','账单打印人数宽度'), ('bp_person_row','1','账单打印人数行数'), ('bp_present','1','赠送金额是否打印'), ('bp_presentflag','0','是否打印赠送标识'), ('bp_price','1','是否打印价格'), ('bp_price_dec','2','价格小位位'), ('bp_price_len','11','价格列宽'), ('bp_price_row','2','价格行数'), ('bp_printmemo','0','是否厨打说明'), ('bp_print_mode','1','账单内容打印方式'), ('bp_receive','1','应收金额是否打印'), ('bp_rowid','0','是否打印行序号'), ('bp_series_group','0','按大类分组'), ('bp_service','0','服务费金额是否打印'), ('bp_specialflag','0','是否打印特价标识'), ('bp_table','1','餐桌的账单打印'), ('bp_table_len','12','账单打印餐桌宽度'), ('bp_table_row','2','账单打印餐桌行数'), ('bp_tel','0','外卖联系电话的账单打印'), ('bp_tel_len','12','账单打印外卖联系电话宽度'), ('bp_tel_row','1','账单打印外卖联系电话行数'), ('bp_time','0','结账时间的账单打印'), ('bp_times','0','时长的账单打印'), ('bp_times_len','12','账单打印时长宽度'), ('bp_times_row','1','账单打印时长行数'), ('bp_time_len','12','账单打印结账时间宽度'), ('bp_time_row','1','账单打印结账时间行数'), ('bp_togo_count','1','外卖帐单连续打印份数'), ('bp_underwrite','1','签名是否打印'), ('bp_unit','0','是否打印单位'), ('bp_unit_len','13','单位列宽'), ('bp_unit_row','2','单位行数'), ('bp_waiter','0','结算服务员打印'), ('bp_waitername','0','服务员名称打印'), ('bp_waitername_len','12','服务员名称打印宽度'), ('bp_waitername_row','1','服务员名称打印行数'), ('bp_waiter_len','12','结算服务员打印宽度'), ('bp_waiter_row','1','结算服务员打印行数'), ('cash_payall','1','现金默认付全款'), ('cp_back','1','厨打退菜标识'), ('cp_billno','0','厨打开台单号'), ('cp_color','0','退菜记录厨打为红色'), ('cp_detailid','1','厨打本单序号'), ('cp_detail_fsize','11','厨打明细字体大小'), ('cp_dishname','1','厨打菜品名称'), ('cp_dishname_row','1','厨打菜品名称的行数'), ('cp_dishname_size','20','厨打菜品名称字符数'), ('cp_dishno','0','厨打菜品编码'), ('cp_dishno_row','2','厨打菜品编码行数'), ('cp_dishno_size','8','厨打菜品编码符数'), ('cp_id','1','厨打序号'), ('cp_num','1','厨打菜品数量'), ('cp_num_dec','2','厨打数量小数位');
INSERT INTO `cey_sys_parameter` VALUES ('cp_num_row','1','厨打菜品数量的行数'), ('cp_num_size','4','厨打菜品数量字符数'), ('cp_offamount','0','表尾厨打金额合计'), ('cp_operator','0','厨打点菜人'), ('cp_ordertime','1','厨打点菜时间'), ('cp_person','0','打印人数'), ('cp_present','1','厨打奉送标识'), ('cp_price','0','厨打菜品价格'), ('cp_price_dec','2','厨打价格小数位'), ('cp_price_row','1','厨打菜品价格的行数'), ('cp_price_size','7','厨打菜品价格字符数'), ('cp_printtime','0','厨打时间'), ('cp_suit_print','3','套菜打印方式'), ('cp_table','1','厨打餐桌名称'), ('cp_title','0','厨打方案名称'), ('cp_title1_fsize','11','厨打表头字体大小'), ('cp_title_1','1','点菜单类型名称'), ('cp_title_fsize','14','厨打标题字体大小'), ('cp_trans_print','0','转台厨打'), ('cp_unit','1','厨打单位'), ('cp_unit_row','1','厨打单位的行数'), ('cp_unit_size','6','厨打单位字符数'), ('cp_width','40','厨打纸张宽度'), ('DataBase_Version','V1_140101','系统数据库版本'), ('handover_oper','0','交班必需接班人'), ('label_address','0','标签打印外卖地址'), ('label_address_line','','标签外卖地址打印所在行'), ('label_billno','0','标签打印单号'), ('label_billno_line','','标签单号打印所在行'), ('label_dishname','0','标签打印品名'), ('label_dishname_line','','标签品名打印所在行'), ('label_memo','0','标签打印做法'), ('label_memo_line','','标签做法打印所在行'), ('label_model','','标签打印模板'), ('label_price','0','标签打印价格'), ('label_price_line','','标签价格打印所在行'), ('label_tel','0','标签打印外卖电话'), ('label_tel_line','','标签外卖电话打印所在行'), ('label_time','0','标签打印时间'), ('label_time_line','','标签时间打印所在行'), ('label_title','0','标签打印抬头'), ('label_title1','0','标签打印标题'), ('label_title1_line','','标签标题打印所在行'), ('label_title_line','','标签抬头打印所在行'), ('low_service','0','服务费记入低消差'), ('negative','0','点菜数量不充许为负数'), ('orderdish_back','0','退菜必须输入退菜原因'), ('orderdish_hf','0','编码点菜启用半模糊查询'), ('ordertonum','0','点菜后自动进入数量录入'), ('pay_auto_discount','0','结账自动折扣'), ('pay_auto_discount_value','100','结账自动折扣值'), ('print_fashion','2','帐单打印发送方式'), ('print_vip_point','0','会员打印积分'), ('print_ye','0','会员打印余额'), ('round_mode','2','四舍五入方式'), ('serial_no','1','是否进行流水号打印'), ('serial_no_begin','0','开始流水号'), ('serial_no_end','999','结束流水号'), ('serial_no_len','12','流水号宽度'), ('serial_no_now','0','开始流水号'), ('serial_no_row','1','流水号列数'), ('service_handwork','0','手工处理服务费'), ('shortorder_tableno','0','快餐必须输入台(牌)号'), ('store_print_total','table','库存单据打印合计金额位置'), ('system_pass','0000','系统管理员密码'), ('sys_business_type','2','经营业态'), ('sys_stock_price','0','用入库价更新采购价'), ('togo_pay_atonce','0','外卖落单立即结账');
INSERT INTO `cey_sys_purview_list` VALUES ('0101','菜品资料维护                  ','N','SHEE','w_bt_dish','数据维护','01','N','NULL'), ('0102','套菜资料维护                  ','N','SHEE','w_bt_dish_suit','数据维护','01','N','NULL'), ('0103','菜品厨打设置                  ','N','SHEE','w_bt_dish_cp','数据维护','01','N','NULL'), ('0104','菜品做法设置                  ','N','SHEE','w_bt_print_memo','数据维护','01','N','NULL'), ('0105','餐桌资料维护                  ','N','SHEE','w_bt_table','数据维护','01','N','NULL'), ('0106','收银方式维护                  ','N','EDIT','w_bt_paymode','数据维护','01','N','NULL'), ('0107','营业班次                      ','N','EDIT','w_bt_class','数据维护','01','N','NULL'), ('0110','特殊操作原因                  ','N','SHEE','w_bt_other','数据维护','01','N','NULL'), ('0201','营业单据查询                  ','N','EDIT','w_u_bill_search','营业管理','02','N','NULL'), ('0202','当前台况统计                  ','N','EDIT','w_u_curtable','营业管理','02','N','NULL'), ('0203','菜品销售流水查询              ','N','SHEE','w_rpt_cost_flow','营业管理','02','N','NULL'), ('0204','收银流水查询                  ','N','SHEE','w_rpt_checkout_flow','营业管理','02','N','NULL'), ('0205','菜品历史特价查询              ','N','SHEE','w_rpt_dish_special','营业管理','02','N','NULL'), ('0206','菜品历史沽清查询              ','N','SHEE','w_rpt_dish_warn','营业管理','02','N','NULL'), ('0207','外卖收银结算                  ','N','EDIT','w_u_togo_balance','营业管理','02','N','NULL'), ('0208','餐台锁态管理                  ','N','EDIT','w_u_table_lock','营业管理','02','N','NULL'), ('0209','开钱箱                        ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0210','开台                          ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0211','转台                          ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0212','消台                          ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0213','点菜                          ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0214','结算                          ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0215','厨房催菜                      ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0216','菜品转台                      ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0217','接班操作                      ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0218','退菜                          ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0219','免服务费                      ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0220','免低消差                      ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0221','预打账单                      ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0222','快餐挂单                      ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0223','快餐外卖                      ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0224','前台营业情况统计              ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0225','外卖退货                      ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0226','增加临时菜                    ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0227','反结账                        ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0228','退单                          ','O','EDIT','NONE','营业管理','02','N','NULL'), ('0240','营业日结                      ','N','EDIT','w_ub_day_sumup','营业管理','02','N','NULL'), ('0241','前台营业                      ','N','SHEE','w_mdi_qt','营业管理','02','N','NULL'), ('0242','今日特价维护                  ','N','SHEE','w_bt_dish_special','营业管理','02','N','NULL'), ('0243','今日菜品沽清                  ','N','SHEE','w_bt_dish_warn','营业管理','02','N','NULL'), ('0401','会员参数设置                  ','N','EDIT','w_m_parameter','会员管理','04','N','NULL'), ('0402','会员资料维护                  ','N','SHEE','w_m_member_list','会员管理','04','N','NULL'), ('0403','会员状态管理                  ','N','SHEE','w_m_manage','会员管理','04','N','NULL'), ('0404','充值会员充值                  ','N','EDIT','w_m_member_deposit','会员管理','04','N','NULL'), ('0406','会员积分管理                  ','N','SHEE','w_m_point_manage','会员管理','04','N','NULL'), ('0407','会员类型变更                  ','N','EDIT','w_m_type_change','会员管理','04','N','NULL'), ('0411','会员报表                      ','N','SHEE','w_rpt_member_total','会员管理','04','N','NULL'), ('0413','会员卡芯片清除                ','N','EDIT','w_m_icblankout','会员管理','04','N','NULL'), ('0602','商品入库                      ','N','SHEE','w_st_voucher_list','库存管理','06','N','RK'), ('0603','商品退货单                    ','N','SHEE','w_st_voucher_list','库存管理','06','N','TH'), ('0604','报损单                        ','N','SHEE','w_st_voucher_list','库存管理','06','N','CK'), ('0605','库存盘点                      ','N','SHEE','w_st_store_check','库存管理','06','N','NULL'), ('0606','库存查询                      ','N','SHEE','w_st_rpt_store_num','库存管理','06','N','NULL'), ('0609','盘点查询                      ','N','SHEE','w_st_rpt_material_check_query','库存管理','06','N','RK'), ('0610','进销存汇总报表                ','N','SHEE','w_st_material_jxc','库存管理','06','N','NULL'), ('0611','单据汇总查询                  ','N','SHEE','w_st_rpt_total','库存管理','06','N','NULL'), ('0612','库存参数设置                  ','N','EDIT','w_st_parameter','库存管理','06','N','NULL'), ('0701','菜品消费报表                  ','N','SHEE','w_rpt_cost','报表中心','07','N','NULL'), ('0702','菜品促销报表                  ','N','SHEE','w_rpt_cost_favourable','报表中心','07','N','NULL'), ('0703','餐桌营业数据统计              ','N','SHEE','w_rpt_cost_tableno','报表中心','07','N','NULL'), ('0704','退菜情况报表                  ','N','SHEE','w_rpt_cost_back','报表中心','07','N','NULL'), ('0705','营业收银报表                  ','N','SHEE','w_rpt_checkout','报表中心','07','N','NULL'), ('0706','服务费与低消情况查询          ','N','SHEE','w_rpt_service_low','报表中心','07','N','NULL'), ('0707','收银免单情况查询              ','N','SHEE','w_rpt_checkout_free','报表中心','07','N','NULL'), ('0708','日月周报表分析                ','N','SHEE','w_rpt_dwm','报表中心','07','N','NULL'), ('0710','日营业情况统计                ','N','SHEE','w_rpt_day','报表中心','07','N','NULL'), ('0711','营业日销售情况统计            ','N','SHEE','w_rpt_day_series','报表中心','07','N','NULL'), ('0712','品类销售营业分析              ','N','SHEE','w_rpt_cost_series','报表中心','07','N','NULL'), ('0713','时段营业分析                  ','N','SHEE','w_rpt_period','报表中心','07','N','NULL'), ('0714','菜品销售排行统计              ','N','SHEE','w_rpt_cost_order','报表中心','07','N','NULL'), ('0715','服务员提成查询                ','N','SHEE','w_rpt_cost_waiter','报表中心','07','N','NULL'), ('0719','反结帐查询                    ','N','SHEE','w_rpt_uncheckout_flow','报表中心','07','N','NULL'), ('0720','直销商品毛利情况表            ','N','SHEE','w_rpt_dish_gross_profit','报表中心','07','N','NULL'), ('0901','公司信息设置                  ','N','EDIT','w_sys_company','系统管理','09','N','NULL'), ('0902','系统参数设置                  ','N','EDIT','w_sys_parameter','营业管理','02','N','NULL'), ('0903','操作员资料维护                ','N','EDIT','w_sys_operator','系统管理','09','N','NULL'), ('0904','操作权限维护                  ','N','EDIT','w_sys_purview','系统管理','09','N','NULL'), ('0905','数据库备份                    ','N','EDIT','w_sys_backup','系统管理','09','N','NULL'), ('0906','基础数据导出                  ','N','EDIT','w_sys_data_export','系统管理','09','N','NULL'), ('0907','基础数据导入                  ','N','EDIT','w_sys_data_import','系统管理','09','N','NULL'), ('0908','业务数据清除                  ','N','EDIT','w_sys_data_clear','系统管理','09','N','NULL'), ('0909','权限设置                      ','N','SHEE','w_sys_purview','系统管理','09','N','NULL'), ('0910','MySQL服务器','N','EDIT',NULL,'系统管理','09','N',NULL), ('0911','文件管理器','N','EDIT',NULL,'系统管理','09','N',NULL), ('0912','无线点菜监控','N','EDIT',NULL,'系统管理','09','N',NULL), ('0913','云同步','N','EDIT',NULL,'系统管理','09','N',NULL), ('1501','短信组管理                    ','N','EDIT','w_ms_group_form','短信管理','15','N','NULL'), ('1502','短信成员管理                  ','N','EDIT','w_ms_cust_input','短信管理','15','N','NULL'), ('1503','短信设置                      ','N','EDIT','w_ms_message_setup','短信管理','15','N','NULL'), ('1504','短信单发                      ','N','EDIT','w_ms_send_single','短信管理','15','N','NULL'), ('1505','短信群发                      ','N','EDIT','w_ms_send_multi','短信管理','15','N','NULL'), ('1506','短信查询                      ','N','EDIT','w_ms_query_send','短信管理','15','N','NULL');
INSERT INTO `cey_sys_sqlversion` VALUES ('1.1','2015-09-01 18:36:40'), ('1.2','2015-09-01 18:36:40'), ('1.3','2015-09-01 18:36:40'), ('1.4','2015-09-01 18:36:40');
INSERT INTO `mess_t_sys_system` VALUES ('auto_time','系统变量auto_time','5'), ('daysum','系统变量daysum','0/23/59/'), ('recommend','系统变量recommend','1');
INSERT INTO `pbcatcol` VALUES ('cey_u_handover_master         ','757577737','dbo                           ','vch_accept                    ','5','','0','','0','0','','0','0','0','','N','','','','');
INSERT INTO `pbcatedt` VALUES ('#####','#####','90','1','1','32','10                              '), ('###,###.00','###,###.00','90','1','1','32','10                              '), ('###-##-####','###-##-####','90','1','1','32','00                              '), ('DD/MM/YY','DD/MM/YY','90','1','1','32','20                              '), ('DD/MM/YY HH:MM:SS','DD/MM/YY HH:MM:SS','90','1','1','32','40                              '), ('DD/MM/YY HH:MM:SS:FFFFFF','DD/MM/YY HH:MM:SS:FFFFFF','90','1','1','32','40                              '), ('DD/MM/YYYY','DD/MM/YYYY','90','1','1','32','20                              '), ('DD/MM/YYYY HH:MM:SS','DD/MM/YYYY HH:MM:SS','90','1','1','32','40                              '), ('DD/MMM/YY','DD/MMM/YY','90','1','1','32','20                              '), ('DD/MMM/YY HH:MM:SS','DD/MMM/YY HH:MM:SS','90','1','1','32','40                              '), ('HH:MM:SS','HH:MM:SS','90','1','1','32','30                              '), ('HH:MM:SS:FFF','HH:MM:SS:FFF','90','1','1','32','30                              '), ('HH:MM:SS:FFFFFF','HH:MM:SS:FFFFFF','90','1','1','32','30                              '), ('JJJ/YY','JJJ/YY','90','1','1','32','20                              '), ('JJJ/YY HH:MM:SS','JJJ/YY HH:MM:SS','90','1','1','32','40                              '), ('JJJ/YYYY','JJJ/YYYY','90','1','1','32','20                              '), ('JJJ/YYYY HH:MM:SS','JJJ/YYYY HH:MM:SS','90','1','1','32','40                              '), ('MM/DD/YY','MM/DD/YY','90','1','1','32','20                              '), ('MM/DD/YY HH:MM:SS','MM/DD/YY HH:MM:SS','90','1','1','32','40                              '), ('MM/DD/YYYY','MM/DD/YYYY','90','1','1','32','20                              '), ('MM/DD/YYYY HH:MM:SS','MM/DD/YYYY HH:MM:SS','90','1','1','32','40                              ');
INSERT INTO `pbcatfmt` VALUES ('#,##0','#,##0','81','0'), ('#,##0.00','#,##0.00','81','0'), ('$#,##0.00;($#,##0.00)','$#,##0.00;($#,##0.00)','81','0'), ('$#,##0.00;[RED]($#,##0.00)','$#,##0.00;[RED]($#,##0.00)','81','0'), ('$#,##0;($#,##0)','$#,##0;($#,##0)','81','0'), ('$#,##0;[RED]($#,##0)','$#,##0;[RED]($#,##0)','81','0'), ('0','0','81','0'), ('0%','0%','81','0'), ('0.00','0.00','81','0'), ('0.00%','0.00%','81','0'), ('0.00E+00','0.00E+00','81','0'), ('d-mmm','d-mmm','84','0'), ('d-mmm-yy','d-mmm-yy','84','0'), ('h:mm AM/PM','h:mm AM/PM','84','0'), ('h:mm:ss','h:mm:ss','84','0'), ('h:mm:ss AM/PM','h:mm:ss AM/PM','84','0'), ('m/d/yy','m/d/yy','84','0'), ('m/d/yy h:mm','m/d/yy h:mm','84','0'), ('mmm-yy','mmm-yy','84','0'), ('[General]','[General]','81','0');
INSERT INTO `pbcattbl` VALUES ('cey_u_handover_master         ','757577737','dbo                           ','-12','400','N','N','134','33','System                          ','-12','400','N','N','134','33','System                          ','-12','400','N','N','134','33','System                          ','');
INSERT INTO `t_ms_comm_used` VALUES ('1','入会得礼品','明媚春天，特别惊喜！举办大型新品推荐活动：邀请会员您携朋友共同参加可领取礼品！时间4 月7 日至10 日，美丽预定热线：XXXX','营销类'), ('2','新店开业信息','尊敬的会员：购买不参加返券活动的化妆品现金满100 元可参加抽奖活动，百分百中奖，奖品最小为面值40 元百货券，惊喜多多！','营销类'), ('3','抽奖促销','4 月8 日至4 月17 日酬宾满200 送100，部分满200 送60 详情请登陆本商场网站：www.xxxx.com，本信息免费','营销类'), ('4','消费满多少参加抽奖','4 月20 日至5 月8 日酬宾满200 送100，部分满200 送60，5 月8 日母亲节有特色活动，详见店内明示.','营销类'), ('5','新品牌推广活动','XX 是充满青春活力的衍生品牌，近期新开店及优惠活动如下：17 日在新品牌隆重开业；此货品购物488，赠精美T恤一件.','营销类'), ('6','店庆促销','即日起至5 月7 日，凡至购买产品满720 元可获赠精美晶莹嫩白礼包一套，购满750 元可额外获赠专业化妆镜一面。','营销类'), ('7','五一促销','五一大酬宾，4 月28 日－5 月7 日化妆品买100 送58；300 元入会会员独享吉祥物好礼；日替换购先到先得。欢迎光临！','营销类'), ('8','端午节祝福','粽子香，香厨房；艾叶香，香满堂；桃枝插在大门上，出门一望麦儿黄；这儿端阳，那儿端阳，处处都端阳。祝福亲爱的朋友端午节快乐','祝福类'), ('9','中秋节祝福','千里试问平安否？且把思念遥相寄。绵绵爱意与关怀，浓浓情意与祝福，中秋快乐！','祝福类'), ('10','生日祝福','亲爱的#cust#sex，您的生日将至，诚邀您携带身份证和会员卡至领取精美手机链并享受一次免费肌肤护理。','营销类'), ('11','新会员礼品','欢迎您成为我们的会员。会员专享杂志《花草语汇》已为您寄出，携此短信至徐家汇形象专柜，可获赠精美体验装一份.','客服类'), ('12','会员奖励','您的会员卡内有新奖励金额为150 元，有效期至2005 年10 月，欢迎您再次光临！','客服类'), ('13','积分兑换','亲爱的#cust会员：售后中心推出积分兑换免费护理活动。详情请咨询.','客服类'), ('14','通知会员领卡','您好！你的会员卡已经做好，请您凭身份证和临时卡到领取。咨询电话：XXXXXX.','客服类');
INSERT INTO `t_ms_group_info` VALUES ('01  ','会员','1 ',NULL), ('02  ','储值用户','1 ',NULL), ('03  ','供应商','1 ',NULL), ('04  ','客户','1 ',NULL);
INSERT INTO `t_m_parameter` VALUES ('deposit_pay_all','0','充值卡付款必须全单满付'), ('discount_view','0','折扣时显示会员折扣详情'), ('input_memberno','1','可手动输入会员号'), ('local_member_select','0','启用本地会员营业时查卡'), ('member_ATT_mode','123','会员需要使用类型'), ('member_bank_print','0','会员作废的时候打印账单'), ('m_card_paydiscount','0','充值卡会员付款时自动折扣'), ('m_deposit_negative','0','充值与实收的金额必须相等'), ('m_fetch_base','0','取款可取保本金额'), ('m_max_deposit','30,000.00','最大充值金额'), ('m_member_pay','0','充值会员付款允许负数'), ('vip_card_type','磁卡','卡类型'), ('vip_ic_baud','','IC卡波特率'), ('vip_ic_machine','明华RF','IC卡读写器'), ('vip_ic_password','FFFFFF','IC卡读写密码'), ('vip_ic_port','USB','读写器接口');
INSERT INTO `t_sys_system` VALUES ('Is_Distributed','系统变量Is_Distributed','1','否',NULL,'1',NULL,NULL), ('pos_enter_new','系统变量 pos_enter_new','C<7:7141..',NULL,NULL,NULL,NULL,NULL), ('pos_validate','系统变量 pos_validate','C<7:7141..',NULL,NULL,NULL,NULL,NULL);

-- ----------------------------
--  Trigger definition for `cey_st_check_master`
-- ----------------------------
DELIMITER ;;
CREATE TRIGGER `tr_st_check_master_update` AFTER UPDATE ON `cey_st_check_master` FOR EACH ROW BEGIN
   DECLARE m_done int default 0;
   DECLARE sheetNo  varchar(12);
   DECLARE materialno varchar(7);
   DECLARE numnum numeric(16,4); 
   DECLARE intflowId integer;
   DECLARE initqty   numeric(16,4);
   DECLARE initprice numeric(16,4);
   DECLARE remainqty numeric(16,4);
   DECLARE remainprice numeric(16,4) ;
   DECLARE cur_rows cursor for
     select b.ch_materialno, b.num_pro_loss from  cey_st_check_detail b 
       where NEW.ch_sheetno = b.ch_sheetno and b.num_pro_loss <> 0
         and b.chg_flag = 'Y'
       order by b.ch_sheetno, b.ch_materialno; 

  DECLARE continue handler for not found set m_done = 1;

  open cur_rows;

  WHILE (m_done = 0) DO 

    fetch cur_rows into materialno, numnum;

     IF(m_done = 0) THEN

      IF(NEW.ch_state = 'Y')THEN

       set sheetNo = NEW.ch_sheetno;

       set intflowId = (select ifnull(max(int_flowId),0) from cey_st_material_flow where ch_materialno = materialno);

        IF (intflowId = 0) THEN
           set initqty = 0;
        ELSE
           set initqty = (select remain_qty from cey_st_material_flow where int_flowId = intflowId);
        END IF;

       IF (intflowId = 0) THEN
           set initprice = 0;
       ELSE
           set initprice = (select remain_price from cey_st_material_flow where int_flowId = intflowId);
       END IF;
        
       set remainqty = initqty + numnum;
       set remainprice = initprice;
        
       IF (numnum > 0) THEN
           insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
                  num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
            values (materialno, now(), 'PD', 1, numnum, 0, remainprice, 0, remainqty, remainprice, sheetNo);
       ELSEIF (numnum < 0) THEN
           insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
                  num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
            values (materialno, now(), 'PD', -1, abs(numnum), 0, remainprice, 0, remainqty, remainprice, sheetNo);
       END IF;
        
       IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN
             update cey_st_material_num set num_num = remainqty, cost_price = remainprice
              where ch_materialno = materialno;
       ELSE
             insert into cey_st_material_num (ch_materialno, num_num, cost_price)
               values (materialno, remainqty, remainprice);
       END IF;
      
      END IF;
        
     END IF;

   END WHILE;

   close cur_rows;

END
;;
DELIMITER ;

-- ----------------------------
--  Trigger definition for `cey_st_sheet_master`
-- ----------------------------
DELIMITER ;;
CREATE TRIGGER `tr_st_sheet_master_update` AFTER UPDATE ON `cey_st_sheet_master` FOR EACH ROW BEGIN
    DECLARE m_de int default 0;
    DECLARE m_done int default 0;
    DECLARE sheetNo  varchar(12);
    DECLARE sheetType varchar(2);
    DECLARE operation varchar(3);
    DECLARE dtaudit datetime;
    DECLARE materialno varchar(7);
    DECLARE numnum numeric(16,4);
    DECLARE numpresent numeric(15,4);
    DECLARE numprice numeric(16,4);
    DECLARE costprice  numeric(16,4);
    DECLARE unitrate numeric(16,2);
    DECLARE intflowId integer; 
    DECLARE initqty   numeric(16,4);
    DECLARE  initprice numeric(16,4);
    DECLARE remainqty numeric(16,4); 
    DECLARE remainprice numeric(16,4);
    DECLARE num_amount numeric(16,2);
    DECLARE cur_rows cursor for
      select b.ch_materialno, b.num_wrapping, b.num_present,  b.num_price, ifnull(c.int_unit_rate,1) 
       from  cey_st_sheet_detail b, cey_bt_dish c
         where NEW.ch_sheetNo = b.ch_sheetno and b.ch_materialno = c.ch_dishno;

   DECLARE continue handler for not found set m_done = 1;

   open cur_rows;

 WHILE (m_done = 0) DO 

    fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;

      IF(NEW.ch_state = 'Y')THEN

        set sheetNo = NEW.ch_sheetNo ;
        set sheetType = NEW.ch_sheetType ;
        set operation = NEW.ch_operation ;
        set dtaudit = NEW.dt_audit ;

        set intflowId = (select ifnull(max(int_flowId),0) from cey_st_material_flow  where ch_materialno = materialno);
        
        IF (intflowId = 0) THEN 
          set initqty =  0 ;
        ELSE
          set initqty = (select remain_qty from cey_st_material_flow where int_flowId = intflowId);
        END IF;

        IF (intflowId = 0) THEN
          set initprice =  0 ;
        ELSE
          set initprice = (select remain_price from cey_st_material_flow where int_flowId = intflowId);
        END IF;

        IF (sheetType = 'RK')  THEN
             
             set remainqty = initqty + numnum;
             
             IF (remainqty = 0) THEN
                set remainprice = 0;
             ELSE
                set remainprice = (abs(((numnum / unitrate) * numprice  + initqty * initprice) / remainqty));
             END IF;
             
             insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
                       num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
               values (materialno, now(), sheetType, 1, numnum, numprice / unitrate, remainqty, 0, remainqty, remainprice, sheetNo);

             IF (numpresent > 0) THEN

                  set initqty = remainqty;
                  set initprice = remainprice;
                  set remainqty = initqty + numpresent;
                  
                  IF (remainqty = 0) THEN
                     set remainprice = 0;
                  ELSEIF (initqty < 0 and remainqty > 0) THEN 
                         set remainprice = numprice / unitrate;
                  ELSE 
	                    set remainprice = abs((initqty * initprice) / remainqty);
                  END IF;
	              
                  insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
                         num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
                  values (materialno, now(), sheetType, 1, numpresent, 0, remainprice, 0, remainqty, remainprice, sheetNo);
             END IF;
             
		
		         IF exists ( Select * from cey_sys_parameter where vch_parameter = 'sys_stock_price' and vch_value = '1') THEN
                update cey_bt_dish set num_sale_price = numprice where ch_dishno = materialno;
             END IF;

             IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN
              update cey_st_material_num set num_num = remainqty, cost_price = remainprice
               where ch_materialno = materialno;
             ELSE 
               insert into cey_st_material_num (ch_materialno, num_num, cost_price)
                 values (materialno, remainqty, remainprice);
             END IF;
  
             fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;
        
        ELSEIF (sheetType = 'TH')  THEN
             set remainqty = initqty - numnum;
             IF (remainqty = 0) THEN
                set remainprice = 0;
             ELSE
                set remainprice = (abs((initqty * initprice - numnum / unitrate * numprice   ) / remainqty));
             END IF;
             
             insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
                       num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
               values (materialno, now(), sheetType, 1, numnum, numprice / unitrate, remainprice, 0, remainqty, remainprice, sheetNo);
             

             IF (numpresent > 0) THEN 
                  set initqty   = remainqty;
                  set initprice = remainprice;
                  set remainqty = initqty - numpresent;
                  
                  IF (remainqty = 0) THEN
                     set remainprice = 0;
                  ELSE
	                   set remainprice = abs((initqty * initprice) / remainqty);
                  END IF;
	              
                  insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
                         num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
                  values (materialno, now(), sheetType, 1, numpresent, 0, remainprice, 0, remainqty, remainprice, sheetNo);
              END IF;

             IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN
              update cey_st_material_num set num_num = remainqty, cost_price = remainprice
               where ch_materialno = materialno;
             ELSE 
               insert into cey_st_material_num (ch_materialno, num_num, cost_price)
                 values (materialno, remainqty, remainprice);
             END IF;
                
             fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;
       
        ELSEIF (sheetType = 'CK') THEN
             set remainqty = initqty - numnum; 
             IF (remainqty = 0) THEN
                set remainprice = 0;
             ELSEIF (initqty < 0 and remainqty > 0) THEN
                set m_de = 1;
             ELSE 
                set remainprice = abs((initqty * initprice - numnum / unitrate * numprice   ) / remainqty);
              END IF;
             
             insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
                       num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
               values (materialno, now(), sheetType, 1, numnum, numprice/unitrate, remainprice, 0, remainqty, remainprice, sheetNo);

             IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN
              update cey_st_material_num set num_num = remainqty, cost_price = remainprice
               where ch_materialno = materialno;
             ELSE 
               insert into cey_st_material_num (ch_materialno, num_num, cost_price)
                 values (materialno, remainqty, remainprice);
             END IF;
             
             fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;
          
        ELSE
            fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;
        END IF;

      END IF;

 END WHILE;

  close cur_rows;

END
;;
DELIMITER ;

-- ----------------------------
--  Trigger definition for `cey_u_orderdish`
-- ----------------------------
DELIMITER ;;
CREATE TRIGGER `tr_cey_u_orderdish_insert` AFTER INSERT ON `cey_u_orderdish` FOR EACH ROW BEGIN
   
   DECLARE flowId integer;
   DECLARE dishno varchar(7);
   DECLARE numnum numeric(10,2);
   DECLARE materialno varchar(7);
   DECLARE intflowId integer;
   DECLARE initqty numeric(16,4);
   DECLARE initprice numeric(16,4);
   DECLARE remainqty numeric(16,4);
   DECLARE remainprice numeric(16,4);
   DECLARE itemflag char(1);

   set flowId = NEW.int_flowId;
   set dishno = NEW.ch_dishno;
   set numnum = NEW.num_num - NEW.num_back;

        set itemflag = 'N';
        set itemflag = (select ifnull(item_flag,'N') from cey_bt_dish where ch_dishno = dishno);
        
	      set materialno = dishno;

        IF (materialno<>'' and itemflag = 'Y') THEN
        
            set intflowId = (select ifnull(max(int_flowId),0) from cey_st_material_flow where ch_materialno = materialno);
        
            IF (intflowId = 0) THEN
                set initqty = 0;
            ELSE
                set initqty = (select remain_qty from cey_st_material_flow where int_flowId = intflowId);
            END IF;

            IF (intflowId = 0) THEN
                set initprice = 0;
            ELSE
                set initprice = (select remain_price from cey_st_material_flow where int_flowId = intflowId);
            END IF;
            
            set remainqty  = initqty - numnum ;

            IF (remainqty = 0) THEN                    
                set remainprice = 0;
            ELSE
                set remainprice = initprice;
            END IF;
            
            insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
                       num_num, sheet_price, cost_price, remain_qty, remain_price, ch_sheetno )
               values (materialno,now(), 'QT', case when numnum > 0 then -1 else 1 end,
                       case when numnum < 0 then -numnum else numnum end, 0, remainprice, remainqty, remainprice, LEFT(flowId,20));


            IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN
                update cey_st_material_num set num_num = remainqty, cost_price = remainprice
                 where ch_materialno = materialno;
            ELSE
                insert into cey_st_material_num (ch_materialno, num_num, cost_price)
                values (materialno, remainqty, remainprice);
            END IF;
        
            END IF;

END
;;
DELIMITER ;
DELIMITER ;;
CREATE TRIGGER `tr_cey_u_orderdish_update` AFTER UPDATE ON `cey_u_orderdish` FOR EACH ROW BEGIN
  DECLARE m_done int default 0; 
  DECLARE flowId integer;
  DECLARE dishno varchar(7);
  DECLARE numnum numeric(10,2);
  DECLARE materialno varchar(7);
  DECLARE intflowId integer;
  DECLARE initqty numeric(16,4);
  DECLARE initprice numeric(16,4);
  DECLARE remainqty numeric(16,4);
  DECLARE remainprice numeric(16,4);
  DECLARE itemflag char(1);

 
IF(NEW.int_flowId = OLD.int_flowId and NEW.num_back > OLD.num_back) THEN
      
  set flowId = NEW.int_flowId;
  set dishno = NEW.ch_dishno;
  set numnum = NEW.num_back - OLD.num_back; 
   
  set itemflag = 'N';
  set itemflag = (select ifnull(item_flag,'N') from cey_bt_dish where ch_dishno = dishno);
	set materialno = dishno;

  IF (materialno<>'' and itemflag = 'Y')  THEN
        
       set intflowId = (select ifnull(max(int_flowId),0) from cey_st_material_flow where ch_materialno = materialno);
        
       IF (intflowId = 0) THEN
            set initqty = 0;
       ELSE
            set initqty = (select remain_qty from cey_st_material_flow where int_flowId = intflowId);
       END IF;

       IF (intflowId = 0) THEN
            set initprice = 0;
       ELSE
            set initprice = (select remain_price from cey_st_material_flow where int_flowId = intflowId);
       END IF;

       set remainqty = initqty + numnum ;

       IF (remainqty = 0)  THEN                    
            set remainprice = 0;
       ELSE
            set remainprice = initprice;
       END IF;
            
       insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
                       num_num, sheet_price, cost_price, remain_qty, remain_price, ch_sheetno )
               values (materialno,now(), 'QT', 1,
                       numnum, 0, remainprice, remainqty, remainprice, left(flowId,20));
            
       IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN
                update cey_st_material_num set num_num = remainqty, cost_price = remainprice
                 where ch_materialno = materialno;
       ELSE
                insert into cey_st_material_num (ch_materialno, num_num, cost_price)
                values (materialno, remainqty, remainprice);
       END IF;
        
    END IF;

      END IF;
   
END
;;
DELIMITER ;

-- ----------------------------
--  Trigger definition for `t_m_deposit`
-- ----------------------------
DELIMITER ;;
CREATE TRIGGER `tr_m_deposit_send` AFTER INSERT ON `t_m_deposit` FOR EACH ROW BEGIN
  DECLARE ls_cardid char(20);  
  DECLARE ln_consum_amt numeric(16,2); 
  DECLARE ls_oper_type char(1);
  DECLARE ls_var_value char(255);
  DECLARE ls_branch_no varchar(10);
  DECLARE g_branch_no varchar(10);
	DECLARE deposit_mode char(1);
	DECLARE ln_point numeric(16,2);
	DECLARE ls_memo	varchar(100);
	DECLARE ll_posit int;

	set deposit_mode = NEW.ch_deposit_mode ;

	IF (deposit_mode = '1' or deposit_mode = '2' or deposit_mode = '7') THEN

	 set ls_branch_no = '0000';

	 IF (deposit_mode = '1') THEN
		
			set ls_var_value = (select sys_var_value  from mess_t_sys_system where Upper(LTRIM(RTRIM(sys_var_id))) = Upper(LTRIM(RTRIM('card_save'))));
			set ls_cardid = NEW.vch_memberno ;
      set ln_consum_amt = NEW.num_deposit;
      set ls_memo = NEW.vch_memo;
				
			IF (length(ls_var_value) <> 0 and ifnull(ls_var_value,'') <> '') THEN
		    IF (left(ls_var_value,1) = '1') THEN
			       insert into mess_t_vip_message(mess_type,branch_no,card_id,savelist_amt,oper_type,send_flag)  values('4',ls_branch_no,ls_cardid,ln_consum_amt,'4','0');
        END IF;
      END IF;
		

	ELSEIF (deposit_mode = '7') THEN
			
		   set ls_var_value = (select ifnull(sys_var_value,'')  FROM mess_t_sys_system where Upper(LTRIM(RTRIM(sys_var_id))) = Upper(LTRIM(RTRIM('vip_giv6'))));				
       IF (length(ls_var_value) <> 0 and ifnull(ls_var_value,'') <> '') THEN
		    	  IF ( left(ls_var_value,1) = '1' ) THEN
					      set ls_cardid = NEW.vch_memberno ;
					      set ln_consum_amt = - NEW.num_deposit;
					      set ls_memo = NEW.vch_memo ;

					     IF (ls_memo is null or ls_memo = '') THEN
						     set ln_point = 0;
					     ELSE
							   set ll_posit = charindex ( ':',ls_memo,0 );
							   set ln_point = format(round( ls_memo, len( ls_memo ) - ll_posit ),2);
               END IF;

					     insert into mess_t_vip_message(mess_type,branch_no,card_id,father_card,giv_acc_num,send_flag,oper_type,change_amt) 
					      values('2',ls_branch_no,ls_cardid,ls_cardid,ln_consum_amt,'0','6',ln_point);
            END IF;
       END IF;
			
END IF;

END IF;

END
;;
DELIMITER ;

-- ----------------------------
--  Trigger definition for `t_m_member`
-- ----------------------------
DELIMITER ;;
CREATE TRIGGER `tr_m_member_update` AFTER UPDATE ON `t_m_member` FOR EACH ROW BEGIN
  DECLARE  ls_cardid char(20);     
  DECLARE  ls_branch_no char(6);
  DECLARE  ls_var_value varchar(255);

	set ls_branch_no = '0000';
		IF (NEW.ch_cardflag = 'Y' ) THEN
		
			set ls_cardid =  NEW.vch_memberno ;

			set ls_var_value = (select ifnull(sys_var_value,'')  from mess_t_sys_system where Upper(LTRIM(RTRIM(sys_var_id))) = Upper(LTRIM(RTRIM('vip_new'))));
				   IF (left(ls_var_value,1) = '1' ) THEN
						  insert into mess_t_vip_message(mess_type,branch_no,card_id,father_card,send_flag)	values('3',ls_branch_no,ls_cardid,'','0');
           END IF;
    END IF;
END
;;
DELIMITER ;

-- ----------------------------
--  Trigger definition for `t_m_pay`
-- ----------------------------
DELIMITER ;;
CREATE TRIGGER `tr_m_pay_send` AFTER INSERT ON `t_m_pay` FOR EACH ROW BEGIN

  DECLARE ls_cardid char(20);       -- 会员卡号
  DECLARE ln_consum_amt numeric(16,4);  -- 充值金额
  DECLARE ls_oper_type char(1);
  DECLARE ls_var_value char(255);
  DECLARE ls_branch_no varchar(10);
  DECLARE g_branch_no varchar(10);

	set ls_branch_no = '0000';

  set ls_var_value = (select sys_var_value from mess_t_sys_system where Upper(LTRIM(RTRIM(sys_var_id))) = Upper(LTRIM(RTRIM('savecard_pay'))));
			
	IF (length(ls_var_value) <> 0 and ifnull(ls_var_value,'') <> '') THEN

      IF (left(ls_var_value,1) = '1') THEN
     
		     set ls_cardid = NEW.vch_memberno ;
         set ln_consum_amt = NEW.num_pay ;
	
	       insert into mess_t_vip_message(mess_type,branch_no,card_id,savelist_amt,oper_type,send_flag) 
	         values('4',ls_branch_no,ls_cardid,ln_consum_amt,'5','0');
      END IF;
	
  END IF; 

END
;;
DELIMITER ;

-- ----------------------------
--  Trigger definition for `t_m_point`
-- ----------------------------
DELIMITER ;;
CREATE TRIGGER `tr_m_point_send` AFTER INSERT ON `t_m_point` FOR EACH ROW BEGIN

  DECLARE ls_cardid 		char(20);      -- 会员卡号
  DECLARE ln_consum_amt 		numeric(16,4);  -- 消费金额
  DECLARE ln_consum_num 		numeric(16,4);  -- 积分
  DECLARE ls_oper_type 		char(1);
  DECLARE ls_var_value 		char(255);
  DECLARE ls_branch_no		varchar(10);
  DECLARE g_branch_no 		varchar(10);
  DECLARE jzbillno	 		varchar(20);
  DECLARE type		    	char(1);

      -- 当前数据
	set ls_cardid = NEW.vch_memberno ;
  set ln_consum_num = NEW.num_point;
  set jzbillno = NEW.ch_payno ;
  set type = NEW.ch_type ;

	set ls_branch_no = '0000';

	IF (type = '2' or type = '1'  or type = '3') THEN

			  set ls_var_value = (select sys_var_value from mess_t_sys_system where Upper(LTRIM(RTRIM(sys_var_id))) = Upper(LTRIM(RTRIM('consumer_auto'))));
					
				IF (length(ls_var_value) <> 0 and ifnull(ls_var_value,'') <> '') THEN
				  IF (left(ls_var_value,1) = '1') THEN
				    IF (jzbillno is not null and jzbillno <> '') THEN
				        set ln_consum_amt = (select num_cost from cey_u_checkout_master where ch_payno = jzbillno); 
            END IF;
						
				   insert into mess_t_vip_message(mess_type,branch_no,card_id,acc_num,flow_amt,oper_type,send_flag,flow_no)
		 		   values ('5',ls_branch_no,ls_cardid,ln_consum_num,ln_consum_amt,'0','0',jzbillno);
          END IF;
			  END IF;
	ELSEIF (type = '4') THEN 
				set ls_var_value = (select ifnull(sys_var_value,'')  FROM mess_t_sys_system WHERE Upper(LTRIM(RTRIM(sys_var_id))) = Upper(LTRIM(RTRIM('vip_giv3'))));
				IF (left(ls_var_value,1) = '1' ) THEN
				  insert into mess_t_vip_message(mess_type,branch_no,card_id,father_card,giv_acc_num,send_flag,oper_type) 
				     values('2',ls_branch_no,ls_cardid,ls_cardid,ln_consum_num,'0','3');
        END IF;
			
  END IF;

END
;;
DELIMITER ;
