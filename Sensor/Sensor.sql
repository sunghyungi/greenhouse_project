select user(), database();

create database test;
create user 'node'@'%' identified by 'node';

use test;

create table if not exists sensor (
	num integer primary key auto_increment,
	time timestamp,
	mac char(20),
	light float(5,1),
	pre float(7,1),
	tem float(4,1),
	hum float(4,1)
);

select * from sensor;
delete from sensor;

drop table if exists sensor;