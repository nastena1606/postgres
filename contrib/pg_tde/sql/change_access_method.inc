CREATE EXTENSION IF NOT EXISTS pg_tde;

SELECT pg_tde_add_key_provider_file('file-vault','/tmp/pg_tde_test_keyring.per');
SELECT pg_tde_set_principal_key('test-db-principal-key','file-vault');

CREATE TABLE country_table (
     country_id        serial primary key,
     country_name    text unique not null,
     continent        text not null
) using :tde_am;
 
INSERT INTO country_table (country_name, continent)
     VALUES ('Japan', 'Asia'),
            ('UK', 'Europe'),
            ('USA', 'North America');

SELECT * FROM country_table;

SELECT pg_tde_is_encrypted('country_table');

-- Try changing the encrypted table to an unencrypted table
ALTER TABLE country_table SET access method  heap;
-- Insert some more data 
INSERT INTO country_table (country_name, continent)
     VALUES ('France', 'Europe'),
            ('Germany', 'Europe'),
            ('Canada', 'North America');

SELECT * FROM country_table;
SELECT pg_tde_is_encrypted('country_table');

-- Change it back to encrypted
ALTER TABLE country_table SET access method  :tde_am;

INSERT INTO country_table (country_name, continent)
     VALUES ('China', 'Asia'),
            ('Brazil', 'South America'),
            ('Australia', 'Oceania');
SELECT * FROM country_table;
SELECT pg_tde_is_encrypted('country_table');

CREATE TABLE country_table2 (
     country_id        serial primary key,
     country_name    text unique not null,
     continent        text not null
);

SET pg_tde.enforce_encryption = ON;

CREATE TABLE country_table3 (
     country_id        serial primary key,
     country_name    text unique not null,
     continent        text not null
) USING heap;
 
ALTER TABLE country_table SET access method  heap;

ALTER TABLE country_table2 SET access method  :tde_am;

CREATE TABLE country_table3 (
     country_id        serial primary key,
     country_name    text unique not null,
     continent        text not null
) using :tde_am;

DROP TABLE country_table;
DROP TABLE country_table2;
DROP TABLE country_table3;

SET pg_tde.enforce_encryption = OFF;

DROP EXTENSION pg_tde;
