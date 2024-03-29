CREATE ROLE smarteco NOSUPERUSER NOCREATEDB NOCREATEROLE INHERIT LOGIN;
CREATE ROLE slm NOSUPERUSER NOCREATEDB NOCREATEROLE INHERIT LOGIN;
CREATE TABLESPACE NASDAT OWNER smarteco location '/data1/NASDAT';
CREATE TABLESPACE NASIDX OWNER smarteco location '/data1/NASIDX';
CREATE TABLESPACE SLMDAT OWNER slm location '/data2/SLMDAT';
CREATE TABLESPACE SLMIDX OWNER slm location '/data2/SLMIDX';
CREATE DATABASE smarteco OWNER smarteco TABLESPACE NASDAT ENCODING 'EUC_KR' TEMPLATE template0;
