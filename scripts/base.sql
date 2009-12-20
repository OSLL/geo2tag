CREATE SEQUENCE marks_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE mark (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('marks_seq'),
  time TIMESTAMP NOT NULL DEFAULT (current_timestamp),
  latitude float NOT NULL,
  longitude float NOT NULL,
  label VARCHAR(1024) NOT NULL,
  description VARCHAR(2048) NOT NULL, 
  constraint mark_pkey primary key (id)
);

CREATE SEQUENCE channels_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE channel (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('channels_seq'),
  description VARCHAR(2048) NOT NULL, 
  constraint channel_pkey primary key (id)
);

CREATE TABLE marks (
  channel_id NUMERIC(9,0) NOT NULL,
  mark_id NUMERIC(9,0) NOT NULL,
  constraint fk_marks  foreign key (mark_id) references mark(id)
                                                       on delete cascade,
  constraint fk_channels foreign key (channel_id) references channel(id)
                                                       on delete cascade
);

