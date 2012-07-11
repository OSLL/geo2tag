CREATE SEQUENCE signup_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE signups (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('signup_seq'),
  datetime TIMESTAMP NOT NULL DEFAULT now(),  
  email VARCHAR(50) UNIQUE NOT NULL,
  login VARCHAR(50) UNIQUE NOT NULL,
  password VARCHAR(50) NOT NULL,
  registration_token VARCHAR(65) UNIQUE NOT NULL,
  sent BOOLEAN NOT NULL DEFAULT FALSE,
  constraint signup_requests_pkey primary key (id)
);

CREATE SEQUENCE users_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE users (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('users_seq'),
  email VARCHAR(50) UNIQUE NOT NULL,
  login VARCHAR(50) UNIQUE NOT NULL,
  password VARCHAR(50) NOT NULL,
  constraint users_pkey primary key (id)
);

CREATE SEQUENCE sessions_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE sessions (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('sessions_seq'),
  user_id NUMERIC(9,0) NOT NULL,
  session_token VARCHAR(65) NOT NULL,
  last_access_time TIMESTAMP NOT NULL DEFAULT now(),
  constraint sessions_pkey primary key (id),
  constraint fk_user foreign key (user_id) references users(id)
                                                      on delete cascade
);


CREATE SEQUENCE channels_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE channel (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('channels_seq'),
  name VARCHAR(300) NOT NULL,
  description VARCHAR(2048) NOT NULL, 
  url VARCHAR(2048) DEFAULT NULL,
  owner_id NUMERIC(9,0) NOT NULL,
  constraint channel_pkey primary key (id),
  constraint fk_owner foreign key (owner_id) references users(id)
                                                      on delete cascade
);

CREATE SEQUENCE tags_seq INCREMENT 1 MINVALUE 1 START 1 CACHE 1;

CREATE TABLE tag (
  id NUMERIC(9,0) NOT NULL DEFAULT nextval('tags_seq'),
  time TIMESTAMP NOT NULL DEFAULT (current_timestamp),
  altitude float NOT NULL,
  latitude float NOT NULL,
  longitude float NOT NULL,
  label VARCHAR(1024) NOT NULL,
  description VARCHAR(2048) NOT NULL, 
  url VARCHAR(2048) DEFAULT NULL, 
  user_id NUMERIC(9,0) NOT NULL DEFAULT 1,
  channel_id NUMERIC(9,0) NOT NULL DEFAULT 1,
  constraint tag_pkey primary key (id),
  constraint fk_user foreign key (user_id) references users(id)
                                                      on delete cascade,
  constraint fk_channel foreign key (channel_id) references channel(id)
                                                      on delete cascade
);

CREATE TABLE subscribe (
  channel_id NUMERIC(9,0) NOT NULL,
  user_id NUMERIC(9,0) NOT NULL,
  constraint uniquenes UNIQUE (channel_id,user_id),
  constraint subscribe_pkey primary key (channel_id,user_id),
  constraint fk_tags  foreign key (user_id) references users(id)
                                                       on delete cascade,
  constraint fk_channels foreign key (channel_id) references channel(id)
                                                       on delete cascade
);

INSERT into users (email, login, password) values ('paul@test.org', 'Paul',   '811e97d9511e53a0da29efee50a8b6468c831606');
INSERT into users (email, login, password) values ('kirill@test.org', 'Kirill', 'b8dcee14ab6a142619fdc5605c9b1182a5183292');
INSERT into users (email, login, password) values ('mark@test.org', 'Mark',   '7b185e162978ee2b6d367c581c5ecda0fcdfbd8a');
INSERT into users (email, login, password) values ('yevgeni@test.org', 'Yevgeni', '47e1b73dab5dbfdef3f2cd531ee44710ab1ec44e');

INSERT into channel (name, description, url, owner_id) values ('Tourist information', 'This is free read-only tourist information channel. You can get information about buildings, sights around your location', '', 1);
INSERT into channel (name, description, url, owner_id) values ('Public announcements', 'This is free read-only channel with public announcements from the city of your current location', '', 1);
INSERT into channel (name, description, url, owner_id) values ('Fuel prices', 'This is free channel where you can post/read the fuel prices around your current location.', '', 1);
INSERT into channel (name, description, url, owner_id) values ('Sales', 'This is free channel where you can post/read sales advertisements', '', 1);
INSERT into channel (name, description, url, owner_id) values ('My channel', 'This is free channel where you and your friends can post/read your tags', '', 1);

INSERT into signups (datetime, email, login, password, registration_token, sent) values (now(), 'email1@test1.org', 'Alex', 'test', 'AAAAAAAAAA', FALSE);
INSERT into signups (datetime, email, login, password, registration_token, sent) values (now(), 'email2@test2.org', 'Kate', 'test', 'KKKKKKKKKK', FALSE);
INSERT into signups (datetime, email, login, password, registration_token, sent) values (now(), 'email3@test3.org', 'Mary', 'test', 'MMMMMMMMMM', FALSE);
INSERT into signups (datetime, email, login, password, registration_token, sent) values (now(), 'email4@test4.org', 'David', 'test', 'DDDDDDDDDD', FALSE);

INSERT into users (email, login, password) values ('rom@test.org', 'Rom',   '6f77a0a131c7a3d07595e06d1b8c9d283f234e47');
INSERT into users (email, login, password) values ('jul@test.org', 'Jul', 'fdfd5c7cf62d92f15e64009e70f25009ffb68590');

INSERT into sessions (user_id, session_token, last_access_time) values (2, 'kkkkkkkkkk', now());
INSERT into sessions (user_id, session_token, last_access_time) values (3, 'mmmmmmmmmm', now());

INSERT into channel (name, description, url, owner_id) values ('St.Petersburg''s sights', 'This is free read-only tourist information channel.', '', 2);
INSERT into channel (name, description, url, owner_id) values ('Route A', '', '', 2);
INSERT into channel (name, description, url, owner_id) values ('Route B', '', '', 2);
INSERT into channel (name, description, url, owner_id) values ('Route C', '', '', 2);
INSERT into channel (name, description, url, owner_id) values ('Route D', '', '', 2);
INSERT into channel (name, description, url, owner_id) values ('Route E', '', '', 2);

INSERT into channel (name, description, url, owner_id) values ('Rom', 'Rom''s channel', '', 3);
INSERT into channel (name, description, url, owner_id) values ('Jul', 'Jul''s channel', '', 3);
INSERT into channel (name, description, url, owner_id) values ('Paul', 'Paul''s channel', '', 3);
INSERT into channel (name, description, url, owner_id) values ('Mark', 'Mark''s channel', '', 3);

INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values(0, 60.166504, 24.841204, 'A', 'Accident at road 51. Time: 15:45, January 2, 2010.', 1, 'http://dps.sd.gov/licensing/driver_licensing/images/Image24.gif',2);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values(0, 60.163216, 24.859314, 'B', 'Shell 95: 1.299 e', 2, 'http://www.unf.edu/groups/volctr/images/question-mark.jpg',3);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values(0,60.166264, 24.859915, 'C', 'Neste Oil diesel: .0989 e', 1, 'http://www.unf.edu/groups/volctr/images/question-mark.jpg',3);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values(0, 60.163216, 24.859915, 'D', 'Neste 95: 1.294 e', 3, 'http://www.unf.edu/groups/volctr/images/question-mark.jpg',3);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values(0, 60.162159, 24.739065, 'E', 'CityMarket Iso Omena: Children overalls 50% off', 1, 'http://www.k-citymarket.fi/img/citymarketLogo.gif',4);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values(0, 60.167433, 24.733486, 'F', 'Prisma Olari: LEGO 20% off', 2, 'http://mw2.google.com/mw-panoramio/photos/thumbnail/5131282.jpg',4);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values(0, 60.173362, 24.829166, 'G', 'Outdoor party place on February 20, 2010', 4, 'http://www.unf.edu/groups/volctr/images/question-mark.jpg',5);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values(0, 60.178308, 24.848585, 'H', 'Espoo. Cheap hotel.', 2, 'http://mw2.google.com/mw-panoramio/photos/small/8267319.jpg',1);



INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values(0,59.938994,30.315857, 'Palace Square', 'Russia, St.Petersburg, Palace Square',1 , 'http://www.saint-petersburg.com/images/virtual-tour/alexander-column.jpg',12);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.934024,30.306182,'Saint Isaac''s Cathedral','The dome of St. Isaac’s Cathedral dominates the skyline of St. Petersburg and its gilded cupola can be seen glistening from all over the city.',2,'http://www.saint-petersburg.com/images/virtual-tour/st-isaac.jpg',6);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.933355,30.320253,'Pushkin Museum', 'Alexander Pushkin Museum and Memorial Apartment',3, 'http://www.saint-petersburg.com/images/museums/pushkin-museum-memorial-apartment.jpg',6);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.934538,30.324301,'Kazan Cathedral','Inspired by the Basilica of St. Peters in Rome, the impressively proportioned Cathedral of Our Lady of Kazan was built just after the turn of the 19th century by the Russian architect Andrei Voronikhin.',1, 'http://static.panoramio.com/photos/large/2508969.jpg',6);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.936482,30.302124,'The Bronze Horseman','This impressive equestrian monument was commissioned by Empress Catherine the Great to honor Peter the Great, the founder of the city.',2, 'http://mw2.google.com/mw-panoramio/photos/medium/831744.jpg',6);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.940044,30.32907, 'Church of the Resurrection of Jesus Christ','Church of the Resurrection of Jesus Christ',3,'http://static.panoramio.com/photos/large/46967036.jpg',6);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.935867,30.320229,'Stroganov Palace','The glorious Stroganov Palace is one of the highlights of Nevsky Prospekt, and is being transformed by the Russian Museum into a tribute to one of Russia''s most influential families.',3,'http://static.panoramio.com/photos/large/30746470.jpg',6);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.925657,30.296165,'Mariinsky Theatre','The Mariinsky Theatre is a historic theatre of opera and ballet in Saint Petersburg, Russia.',3, 'http://static.panoramio.com/photos/large/29266433.jpg',6);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.945152,30.371842,'Tauride Garden','Located in the grounds of the Tauride Palace, formerly home to Catherine the Great''s favorite Grigory Potemkin, this park was seriously damaged during the Siege of Leningrad, but has since been restored.', 3, 'http://www.saint-petersburg.com/images/parks/tauride-garden.jpg',6);




INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.935054,30.329731,'A0','Nevsky Prospekt (Saint Petersburg Metro)', 2, '',14);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.935344,30.326276,'A1','Kazansky Bridge Standing below the east colonnade of Kazan Cathedral, the Kazansky Bridge carries Nevsky Prospekt across the Griboedov Canal.', 2, 'http://www.saint-petersburg.com/images/bridges/kazansky-bridge.jpg',7);

INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.937107,30.31183,'A2','Nevsky Prospekt,2 ', 2, '',7);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.937763,30.313035,'A3','Dvortsovaya pl.,10', 2, '',7);

INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.940955,30.311701,'A4','Dvortsovaya Naberezhnaya,30 ', 2, '',7);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.942159,30.322859,'A5','Moshkov per.,8', 2, '',7);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.935844,30.322167,'A6','Nevsky Prospekt,22', 2, '',7);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.935054,30.329731,'A7','Nevsky Prospekt (Saint Petersburg Metro)', 2, '',7);


INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.933925,30.333441,'B0','Gostiny Dvor(Saint Petersburg Metro)', 2, '',15);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.931571,30.329236,'B1','Lomonosov str.,6', 2, '',8);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.928345,30.335888,'B2','Lomonosov Bridge ', 2, 'http://www.saint-petersburg.com/images/bridges/lomonosov-bridge.jpg',8);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.93328,30.343269,'B3','Anichkov Bridge ', 2, 'http://www.saint-petersburg.com/images/bridges/anichkov-bridge.jpg',8);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.933925,30.333441,'B4','Gostiny Dvor(Saint Petersburg Metro)', 2, '',8);


INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.944528,30.359879,'C0','Chernyshevskaya(Saint Petersburg Metro)', 2, '',9);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.945189,30.360972,'C1','Furshatskaya str.', 2, '',9);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.945152,30.371842,'C2','Tauride Garden', 2, 'http://www.saint-petersburg.com/images/parks/tauride-garden.jpg',9);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.94376,30.368825,'C3','Kirochnaya str.', 2, '',9);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.944528,30.359879,'C4','Chernyshevskaya(Saint Petersburg Metro)', 2, '',9);



INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.928254,30.345996,'D0','Dostoevskaya(Saint Petersburg Metro)', 2, '',10);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.928361,30.34753,'D1','Vladimirsky prosp.', 2, '',10);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.932796,30.347069,'D2','Nevsky Prospekt', 2, '',10);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.931753,30.345502,'D3','Rubenshtein str.', 2, '',10);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.928254,30.345996,'D4','Dostoevskaya(Saint Petersburg Metro)', 2, '',10);


INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.952021,30.291339,'E0','Sportivnaya(Saint Petersburg Metro)', 2, '',13);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.949488,30.286279,'E1','Bolshoi prosp.', 2, '',11);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.946049,30.291277,'E2','Naberezhnaya Makarova', 2, '',11);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.946382,30.303379,'E3','', 2, '',11);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.950229,30.297693,'E4','', 2, '',11);
INSERT into tag(altitude, latitude, longitude, label, description, user_id, url, channel_id) values (0,59.952021,30.291339,'E5','Sportivnaya(Saint Petersburg Metro)', 2, '',11);


INSERT into subscribe (channel_id, user_id) values (12, 5);
INSERT into subscribe (channel_id, user_id) values (13, 5);
INSERT into subscribe (channel_id, user_id) values (13, 6);
INSERT into subscribe (channel_id, user_id) values (12, 6);
INSERT into subscribe (channel_id, user_id) values (14, 1);
INSERT into subscribe (channel_id, user_id) values (15, 3);

