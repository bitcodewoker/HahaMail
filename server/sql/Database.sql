# Create Database
drop database if exists email_system;
create database email_system;
use email_system;

# Create user and set privileges
drop user if exists 'email_admin'@'%';
create user 'email_admin'@'%' identified by 'email_admin_passwd';
grant all on email_system.* to 'email_admin'@'%';

# Create tables
# Create account info table, which contains basic info of every account
drop table if exists email_system.account_info;
create table account_info (
    id                char(16)     not null,
    host              varchar(30)  not null,
    domain            varchar(30)  not null,
    password          varchar(30)  not null,
    nickname          varchar(30)           default '',
    description       varchar(100)          default '',
    recovery_question varchar(100) not null,
    recovery_answer   varchar(100) not null,
    locked            int          not null default 0,
    primary key (id)
) charset = UTF8MB4;
drop table if exists account_activity;
create table account_activity (
    id           char(16)    not null,
    account_name varchar(70) not null,
    # Client IP
    ip           varchar(15) not null,
    action       int         not null default -1,
    time         timestamp   not null,
    status       int         not null default 0,
    primary key (id)
) charset = UTF8MB4;
drop table if exists account_token;
create table account_token (
    token       char(16)    not null,
    account_id  varchar(16) not null,
    ip          varchar(15) not null,
    create_time timestamp   not null,
    expire_time timestamp   not null,
    status      int         not null default 0,
    primary key (token),
    foreign key (account_id) references account_info (id) on delete cascade on update cascade
) charset = UTF8MB4;
drop table if exists accessory;
create table accessory (
    id char(16) not null,
    # TODO: add accessory column
    primary key (id)
) charset = UTF8MB4;
drop table if exists email;
create table email (
    id                char(16)     not null,
    sender_id         char(16)     not null,
    recipient_id      char(16)     not null,
    time              timestamp    not null,
    title             varchar(100) not null default '默认标题',
    body              varchar(10000)        default '',
    accessory_num     int                   default 0,
    accessory_id_list char(160)             default '',
    status            int          not null,
    primary key (id),
    foreign key (sender_id) references account_info (id) on delete cascade on update cascade,
    foreign key (recipient_id) references account_info (id) on delete cascade on update cascade
) charset = UTF8MB4;
drop table if exists draft;
create table draft (
    id                char(16)     not null,
    sender_id         char(16)     not null,
    recipient_name    char(100)             default '',
    time              timestamp    not null,
    title             varchar(100) not null default '默认标题',
    body              varchar(10000)        default '',
    accessory_num     int                   default 0,
    accessory_id_list char(160)             default '',
    primary key (id),
    foreign key (sender_id) references account_info (id) on delete cascade on update cascade
) charset = UTF8MB4;

drop table if exists contact;
create table contact (
                         owner_id   char(16) not null,
                         contact_id char(16) not null,
                         alias      varchar(30) default '',
                         foreign key (owner_id) references account_info (id) on delete cascade on update cascade,
                         foreign key (contact_id) references account_info (id) on delete cascade on update cascade
) charset = UTF8MB4;
