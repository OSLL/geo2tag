CREATE SEQUENCE marks_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE marks (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('marks_seq'),
  time TIMESTAMP NOT NULL DEFAULT (current_timestamp),
  latitude float NOT NULL,
  longitude float NOT NULL,
  label VARCHAR(1024) NOT NULL,
  description VARCHAR(2048) NOT NULL, 
  PRIMARY KEY (id)
);

