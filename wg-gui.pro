TEMPLATE = subdirs

SUBDIRS = \
	wireguard \
        app

app.subdir = src/app
wireguard.subdir = src/wireguard

app.depends = wireguard

