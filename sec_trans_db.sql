create database SEC_TANS_DB;

use SEC_TANS_DB;

# 创建网点信息表
CREATE TABLE SEC_TANS_DB.SECNODE(
	id     				char(4) PRIMARY KEY, 
	name     			char(20) NOT NULL,
 	nodedesc      		char(50),
 	createtime			date,
 	authcode			numeric(12),
 	state     			numeric(4)
);

# 插入两条记录
INSERT INTO SEC_TANS_DB.SECNODE VALUES('0001', '成都银行', '四川成都', '2015-6-10', 1, 0);
INSERT INTO SEC_TANS_DB.SECNODE VALUES('1111', '江西银行', '江西南昌', '2020-9-3', 1111, 0);
commit;

# 创建密钥信息表
CREATE TABLE SEC_TANS_DB.SECKEYINFO(
	clientid          	char(4) ,
	serverid          	char(4) ,
	keyid            	numeric PRIMARY KEY, 
	createtime			date,
	state				numeric,
	seckey				varchar(256)
);

# 密钥ID信息表
CREATE TABLE SEC_TANS_DB.KEYSN(
	ikeysn 		numeric  PRIMARY KEY
);

select ikeysn from SEC_TANS_DB.KEYSN for update
