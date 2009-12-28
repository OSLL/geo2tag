CREATE SEQUENCE users_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE users (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('users_seq'),
  login VARCHAR(50) NOT NULL,
  password VARCHAR(50) NOT NULL,
  constraint users_pkey primary key (id)
);

CREATE SEQUENCE tags_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE tag (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('tags_seq'),
  time TIMESTAMP NOT NULL DEFAULT (current_timestamp),
  latitude float NOT NULL,
  longitude float NOT NULL,
  label VARCHAR(1024) NOT NULL,
  description VARCHAR(2048) NOT NULL, 
  url VARCHAR(2048) DEFAULT NULL, 
  user_id NUMERIC(9,0) NOT NULL DEFAULT 1,
  constraint tag_pkey primary key (id),
  constraint fk_user foreign key (user_id) references users(id)
                                                      on delete cascade
);

CREATE SEQUENCE channels_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE channel (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('channels_seq'),
  name VARCHAR(300) NOT NULL,
  description VARCHAR(2048) NOT NULL, 
  url VARCHAR(2048) DEFAULT NULL,
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


INSERT into channel (name, description, url) values ('weather', 'Tags with wether', '');
INSERT into channel (name, description, url) values ('sales', 'Sales', '');
INSERT into channel (name, description, url) values ('petroleum costs', 'Petroleum stations, prices', '');
INSERT into channel (name, description, url) values ('announcements', 'Art, cinema, show announcements', '');
INSERT into channel (name, description, url) values ('personal', 'User personal tags', '');
INSERT into channel (name, description, url) values ('government', 'tags connected to government', '');

INSERT into users (login, password) values ('test0', 'test');
INSERT into users (login, password) values ('test1', 'test');
INSERT into users (login, password) values ('test2', 'test');
INSERT into users (login, password) values ('test3', 'test');


