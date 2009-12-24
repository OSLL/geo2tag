CREATE SEQUENCE tags_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE tag (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('tags_seq'),
  time TIMESTAMP NOT NULL DEFAULT (current_timestamp),
  latitude float NOT NULL,
  longitude float NOT NULL,
  label VARCHAR(1024) NOT NULL,
  description VARCHAR(2048) NOT NULL, 
  url VARCHAR(2048) DEFAULT NULL, 
  constraint tag_pkey primary key (id)
);

CREATE SEQUENCE channels_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE channel (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('channels_seq'),
  description VARCHAR(2048) NOT NULL, 
  constraint channel_pkey primary key (id)
);

CREATE TABLE tags (
  channel_id NUMERIC(9,0) NOT NULL,
  tag_id NUMERIC(9,0) NOT NULL,
  constraint fk_tags  foreign key (tag_id) references tag(id)
                                                       on delete cascade,
  constraint fk_channels foreign key (channel_id) references channel(id)
                                                       on delete cascade
);

