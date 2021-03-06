#include <sys/mman.h>
#include <sys/stat.h>
#include "lib_acl.h"

static const char *__data1 =
  "<?xml version=\"1.0\"?>\r\n"
  "<?xml-stylesheet type=\"text/xsl\"\r\n"
  "\thref=\"http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl\"?>\r\n"
  "\t<!DOCTYPE refentry PUBLIC \"-//OASIS//DTD DocBook XML V4.1.2//EN\"\r\n"
  "\t\"http://www.oasis-open.org/docbook/xml/4.1.2/docbookx.dtd\" [\r\n"
  "	<!ENTITY xmllint \"<command>xmllint</command>\">\r\n"
  "]>\r\n"
  "<root name='root1' id='root_id_1'>\r\n"
  "  <user name='user11\"' value='zsx11' id='id11'> user zsx11 </user>\r\n"
  "  <user name='user12' value='zsx12' id='id12'> user zsx12 \r\n"
  "    <age year='1972'>my age</age>\r\n"
  "    <other>\r\n"
  "      <email name='zsxxsz@263.net'/>"
  "      <phone>"
  "        <mobile number='111111'> mobile number </mobile>"
  "        <office number='111111'> mobile number </office>"
  "      </phone>"
  "    </other>"
  "  </user>\r\n"
  "  <user name='user13' value='zsx13' id='id13'> user zsx13 </user>\r\n"
  "</root>\r\n"
  "<root name='root2' id='root_id_2'>\r\n"
  "  <user name='user21' value='zsx21' id='id21'> user zsx21 </user>\r\n"
  "  <user name='user22' value='zsx22' id='id22'> user zsx22 \r\n"
  "    <!-- date should be the date of the latest change or the release version -->\r\n"
  "    <age year='1972'>my age</age>\r\n"
  "  </user>\r\n"
  "  <user name='user23' value='zsx23' id='id23'> user zsx23 </user>\r\n"
  "</root>\r\n"
  "<root name = 'root3' id = 'root_id_3'>\r\n"
  "  <user name = 'user31' value = 'zsx31' id = 'id31'> user zsx31 </user>\r\n"
  "  <user name = 'user32' value = 'zsx32' id = 'id32'> user zsx32 </user>\r\n"
  "  <user name = 'user33' value = 'zsx33' id = 'id33'> user zsx33 \r\n"
  "    <age year = '1978' month = '12' day = '11'> bao bao </age>\r\n"
  "  </user>\r\n"
  "  <!-- still a bit buggy output, will talk to docbook-xsl upstream to fix this -->\r\n"
  "  <!-- <releaseinfo>This is release 0.5 of the xmllint Manual.</releaseinfo> -->\r\n"
  "  <!-- <edition>0.5</edition> -->\r\n"
  "  <user name = 'user34' value = 'zsx34' id = 'id34'> user zsx34 </user>\r\n"
  "</root>\r\n";

static const char *__data2 =
  "<?xml version=\"1.0\"?>\r\n"
  "<?xml-stylesheet type=\"text/xsl\"\r\n"
  "	href=\"http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl\"?>\r\n"
  "<!DOCTYPE refentry PUBLIC \"-//OASIS//DTD DocBook XML V4.1.2//EN\"\r\n"
  "  \"http://www.oasis-open.org/docbook/xml/4.1.2/docbookx.dtd\" [\r\n"
  "  <!ENTITY xmllint \"<command>xmllint</command>\">\r\n"
  "]>\r\n"
  "<root>test\r\n"
  "  <!-- <edition> - <!--0.5--> - </edition> -->\r\n"
  "  <user>zsx\r\n"
  "    <age>38</age>\r\n"
  "  </user>\r\n"
  "</root>\r\n"
  "<!-- <edition><!-- 0.5 --></edition> -->\r\n"
  "<!-- <edition>0.5</edition> -->\r\n"
  "<!-- <edition> -- 0.5 -- </edition> -->\r\n"
  "<root name='root' id='root_id'>test</root>\r\n";

static const char *__data3 = \
  "<root id='tt' >hello <root2> hi </root2></root>\r\n";

static const char* __data4 = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n"
  "<request action=\"get_location\" sid=\"YOU_CAN_GEN_SID\" user=\"admin@test.com\">\r\n"
  "  <tags1>\r\n"
  "    <module name=\"mail_ud_user\" />\r\n"
  "  </tags1>\r\n"
  "  <tags2>\r\n"
  "    <tags21/>\r\n"
  "    <tags22/>\r\n"
  "    <tags23/>\r\n"
  "    <module name=\"mail_ud_user\" />\r\n"
  "  </tags2>\r\n"
  "  <tag3>\r\n"
  "    <module name=\"mail_ud_user\"></module>\r\n"
  "  </tag3>\r\n"
  "  <tag4>\r\n"
  "    <module name=\"mail_ud_user\"/>\r\n"
  "  </tag4>\r\n"
  "</request>\r\n"
  "<!-- <edition> -- 0.5 -- </edition> -->\r\n"
  "<!-- <edition> -- 0.5 -- </edition> -->\r\n"
  "<!-- <edition> -- 0.5 -- </edition> -->\r\n";

static const char* __data5 = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n"
  "<request action=\"get_location\" sid=\"YOU_CAN_GEN_SID\" user=\"admin@test.com\">\r\n"
  "  <tag3>\r\n"
  "    <module name=\"mail_ud_user\" />\r\n"
  "  </tag3>\r\n"
  "  <tag4>\r\n"
  "    <module name=\"mail_ud_user\" />\r\n"
  "  </tag4>\r\n"
  "</request>\r\n";

static const char* __data6 = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n"
  "<request action=\"get_location\" sid=\"YOU_CAN_GEN_SID\" user=\"admin@test.com\">\r\n"
  "  <tags2>\r\n"
  "    <tags21>\r\n"
  "    <tags22>\r\n"
  "    <tags23 />\r\n"
  "    <tags24 />\r\n"
  "    <tags25>\r\n"
  "    <tags26/>\r\n"
  "    <tags27>\r\n"
  "    <tags28/>\r\n"
  "    <tags29>\r\n"
  "    <tags30>\r\n"
  "  </tags2>\r\n"
  "</request>\r\n";

static const char* __data7 = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n"
  "<request action=\"get_location\" sid=\"YOU_CAN_GEN_SID\" user=\"admin@test.com\">\r\n"
  "  <tags2>\r\n"
  "   <tags22>\r\n"
  "   <tags23>\r\n"
  "   <tags24>\r\n"
  "   <tags25/>\r\n"
  "   <tags26/>\r\n"
  "   <tags27>\r\n"
  "   <tags28>\r\n"
  "   <tags29/>\r\n"
  "   <tags30>\r\n"
  "   <tags31>\r\n"
  "  </tags2>\r\n"
  "</request>\r\n";

static char *mmap_addr(int fd, size_t len)
{
	char *ptr;

	ptr = (char*) mmap(NULL, len, PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);
	if (ptr == NULL) {
		printf("mmap error %s\r\n", acl_last_serror());
		exit (1);
	}

	return ptr;
}

static void ummap_addr(char *addr, int len)
{
	munmap(addr, len);
}

static void parse_xml_benchmark(int max, const char *data)
{
	int   i;
	ACL_XML3 *xml = acl_xml3_alloc();

	acl_xml3_slash(xml, 1);

	ACL_METER_TIME("-------------bat begin--------------");
	for (i = 0; i < max; i++) {
		char *ptr = acl_mystrdup(data);
		acl_xml3_parse(xml, ptr);
		acl_myfree(ptr);
		acl_xml3_reset(xml);
	}

	ACL_METER_TIME("-------------bat end--------------");
	acl_xml3_free(xml);

	printf("--------------benchmark_max: %d--------------\r\n", max);

	printf("Enter any key to continue ...\r\n");
	getchar();
}

static int parse_xml_file(const char *filepath)
{
	acl_int64 len;
	ACL_VSTREAM *fp = acl_vstream_fopen(filepath, O_RDWR, 0600, 8192);
	char *addr;
	ACL_XML3 *xml;

	if (fp == NULL) {
		printf("open %s error %s\r\n", filepath, acl_last_serror());
		return -1;
	}
	len = acl_vstream_fsize(fp);
	if (len <= 0) {
		printf("fsize %s error %s\r\n", filepath, acl_last_serror());
		acl_vstream_close(fp);
		return -1;
	}

	addr = mmap_addr(ACL_VSTREAM_SOCK(fp), (size_t) len);
	if (addr == NULL)
	{
		printf("mmap_addr error %s\r\n", acl_last_serror());
		acl_vstream_close(fp);
		return -1;
	}
	xml = acl_xml3_alloc();

	ACL_METER_TIME("-------------begin--------------");
	acl_xml3_update(xml, addr);
	ACL_METER_TIME("-------------end--------------");

	acl_vstream_close(fp);

	if (acl_xml3_is_complete(xml, "root"))
		printf("Xml is complete OK, filepath: %s\r\n", filepath);
	else
		printf("Xml is not complete, filepath: %s\r\n", filepath);
	acl_xml3_free(xml);

	ummap_addr(addr, len);

	printf("Enter any key to continue ...\r\n");
	getchar();

	return 0;
}

static void walk_xml(ACL_XML3* xml)
{
	ACL_ITER iter1;

	/* ???????????????? xml ?????????????? */

	printf("-------------- walk_xml -----------------------\r\n");

	acl_foreach(iter1, xml) {
		int  i;
		ACL_ITER iter2;
		ACL_XML3_NODE *node = (ACL_XML3_NODE*) iter1.data;

		for (i = 1; i < node->depth; i++)
			printf("\t");

		printf("tag->%s, size: %ld\n", node->ltag,
			(long) node->ltag_size);
		if (node->ltag_size != strlen(node->ltag)) {
			printf("ltag_size invalid\r\n");
			exit (1);
		}

		/* ???? xml ?????????? */
		acl_foreach(iter2, node->attr_list) {
			ACL_XML3_ATTR *attr = (ACL_XML3_ATTR*) iter2.data;

			for (i = 1; i < node->depth + 1; i++)
				printf("\t");

			printf("attr->%s=\"%s\"\n", attr->name, attr->value);
		}

		for (i = 1; i < node->depth + 1; i++)
			printf("\t");
		printf("text->%s, size: %ld\n", node->text,
			(long) node->text_size);
	}

	printf("-------------- walk_xml end -------------------\r\n");

	printf("Enter any key to continue ...\r\n");
	getchar();
}

static void xml_node_attrs(ACL_XML3_NODE* node, int n)
{
	int i;
	ACL_ITER iter;

	acl_foreach(iter, node->attr_list) {
		ACL_XML3_ATTR *attr = (ACL_XML3_ATTR*) iter.data;

		for (i = 0; i < n; i++)
			printf("\t");

		printf("attr-><%s>(size=%ld)=\"<%s>(size=%ld)\"\n",
			attr->name, (long) attr->name_size,
			attr->value, (long) attr->value_size);

		if (attr->name_size != strlen(attr->name)) {
			printf("%s(%d): name_size invalid\r\n",
				__FUNCTION__, __LINE__);
			exit (1);
		}
		if (attr->value_size != strlen(attr->value)) {
			printf("%s(%d): value_size invaid\r\n",
				__FUNCTION__, __LINE__);
			exit (1);
		}
	}
}

static void walk_xml_node(ACL_XML3_NODE *node, int n)
{
	ACL_ITER iter;

	/* ???????????????? */

	acl_foreach(iter, node) {
		int   i;
		ACL_XML3_NODE *child = (ACL_XML3_NODE*) iter.data;

		for (i = 0; i < n; i++)
			printf("\t");

		printf("tag->%s, size: %ld, %ld\n", child->ltag,
			(long) child->ltag_size, (long) strlen(child->ltag));

		if (child->ltag_size != strlen(child->ltag)) {
			printf("%s(%d): ltag_size invalid\r\n",
				__FUNCTION__, __LINE__);
			exit (1);
		}

		xml_node_attrs(child, n + 1);

		for (i = 0; i < n + 1; i++)
			printf("\t");

		printf("text->%s, size: %ld, %ld\n", child->text,
			(long) child->text_size, (long) strlen(child->text));

		if (child->text_size != strlen(child->text)) {
			printf("%s(%d): text_size invalid\r\n",
				__FUNCTION__, __LINE__);
			exit (1);
		}

		walk_xml_node(child, n + 1);
	}
}

static void list_xml_tags(ACL_XML3 *xml)
{
	ACL_ITER iter;

	printf("-------------- list xml's all tags --------------------\r\n");
	acl_foreach(iter, xml) {
		ACL_XML3_NODE *node = (ACL_XML3_NODE*) iter.data;
		printf(">>tag: %s\n", node->ltag);
	}

	printf("-------------- list xml's all tags end ----------------\r\n");

	printf("Enter any key to continue ...\r\n");
	getchar();
}

static void test_getElementsByTagName(ACL_XML3 *xml, const char *tag)
{
	ACL_ARRAY *a;
	ACL_ITER iter;

	printf("--------- acl_xml3_getElementsByTagName ------------\n");

	a = acl_xml3_getElementsByTagName(xml, tag);
	if (a) {
		/* ?????????? */
		acl_foreach(iter, a) {
			ACL_XML3_NODE *node = (ACL_XML3_NODE*) iter.data;
			printf("tag->%s, text: %s\n", node->ltag, node->text);
			xml_node_attrs(node, 1);
			walk_xml_node(node, 1);
		}

		/* ???????????? */
		acl_xml3_free_array(a);
	}
	else
		printf(">>> not exist, tag: %s\r\n", tag);

	printf("--------- acl_xml3_getElementsByTagName end --------\n");

	printf("Enter any key to continue ...\r\n");
	getchar();
}

static ACL_XML3_NODE *test_getElementById(ACL_XML3 *xml, const char *id)
{
	ACL_ITER iter1;
	ACL_XML3_NODE *node = acl_xml3_getElementById(xml, id);

	printf("--------------- acl_xml3_getElementById ---------------\r\n");
	if (node == NULL) {
		printf(">>>id: %s not found\r\n", id);
		return NULL;
	}

	printf("tag-> %s, text: %s\n", node->ltag, node->text);

	/* ?????? xml ?????????? */
	acl_foreach(iter1, node->attr_list) {
		ACL_XML3_ATTR *attr = (ACL_XML3_ATTR*) iter1.data;
		printf("\tattr_name: %s, attr_value: %s\n",
			attr->name, attr->value);
	}

	printf("----------- the id2_2's next node is ------------\n");

	node = acl_xml3_node_next(node);

	if (node) {
		printf("-------------- walk node ----------------\n");

		/* ?????? xml ?????????? */
		acl_foreach(iter1, node->attr_list) {
			ACL_XML3_ATTR *attr = (ACL_XML3_ATTR*) iter1.data;
			printf("\tattr_name: %s, attr_value: %s\n",
				attr->name, attr->value);
		}

	} else
		printf("-------------- null node ----------------\n");

	printf("Enter any key to continue ...\r\n");
	getchar();

	return node;
}

static ACL_XML3 *get_xml(char *data, const char* root, int multi_root)
{
	ACL_XML3 *xml;
	const char *left;

	printf("--------------------- xml data ------------------------\r\n");
	printf("{%s}\r\n", data);
	printf("--------------------- xml data end --------------------\r\n");
	printf("Enter any key to continue ...\r\n");
	getchar();

	xml = acl_xml3_alloc();
	acl_xml3_multi_root(xml, multi_root);
	acl_xml3_slash(xml, 1);

	/* ???????????????? xml ???? */
	ACL_METER_TIME("-------------parse begin--------------");
	left = acl_xml3_update(xml, data);

	ACL_METER_TIME("-------------parse end----------------");
	printf("---------------- left data ----------------------------\r\n");
	printf("{%s}\r\n", left ? left : "empty");
	printf("---------------- left data end ------------------------\r\n");

	printf("Enter any key to continue ...\r\n");
	getchar();

	if (acl_xml3_is_complete(xml, root))
		printf(">> Yes, the xml complete\n");
	else
		printf(">> No, the xml not complete, root tag: %s\n", root);

	printf("Enter any key to continue ...\r\n");
	getchar();

	return xml;
}

static void parse_xml(const char *data, const char* root, int multi_root)
{
	char *buf = acl_mystrdup(data);
	ACL_XML3 *xml = get_xml(buf, root, multi_root);
	ACL_XML3_NODE *node;
	int total = xml->node_cnt, left;

	/* ???????? xml ???? */
	walk_xml(xml);

	/* ???????? root ???????????????? */
	printf("-------------------- walk root node -------------------\r\n");
	walk_xml_node(xml->root, 0);
	printf("-------------------- walk root node end ---------------\r\n");

	printf("Enter any key to continue ...\r\n");
	getchar();

	/* ?????????????? xml ???????? */

	/* ???????????? name, ???????? user ?????? xml ?????????? */
	test_getElementsByTagName(xml, "user");

	/* ???????????? name, ???????? user2_1 ?????? xml ?????????? */
	test_getElementsByTagName(xml, "user2_1");

	/* ???????????? id, ???????? id2_2 ?????? xml ???????? */
	(void) test_getElementById(xml, "id2_2");

	/* ???????????? id, ???????? id2_3 ?????? xml ???????? */
	node = test_getElementById(xml, "id2_3");
	if (node) {
		int   ndel = 0, node_cnt;

		/* ???????????????????? */
		printf(">>>before delete %s, total: %d\n",
			node->ltag, xml->node_cnt);
		ndel = acl_xml3_node_delete(node);
		node_cnt = xml->node_cnt;
		printf(">>>after delete id2_3(%d deleted), total: %d\n",
			ndel, node_cnt);
	}
	node = test_getElementById(xml, "id2_3");

	list_xml_tags(xml);

	/* ???? xml ???? */
	left = acl_xml3_free(xml);

	acl_myfree(buf);

	printf("Free all node ok, total(%d), left is: %d\n", total, left);
}

static void test1(void)
{
	const char* data = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n"
	"<?xml-stylesheet type=\"text/xsl\"\r\n"
	"\thref=\"http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl\"?>\r\n"
	"<!DOCTYPE refentry PUBLIC \"-//OASIS//DTD DocBook XML V4.1.2//EN\"\r\n"
	"\t\"http://www.oasis-open.org/docbook/xml/4.1.2/docbookx.dtd\" [\r\n"
	"	<!ENTITY xmllint \"<command>xmllint</command>\">\r\n"
	"]>\r\n"
	"<root name1 = \"value1\" name2 = \"val\\ue2\" name3 = \"v\\al'ue3\">hello world!</root>\r\n";
	ACL_XML3 *xml;
	ACL_XML3_NODE *node;
	const char *encoding, *type, *href;
	char *buf = acl_mystrdup(data);

	xml = acl_xml3_alloc();

	printf("---------------- xml data ----------------------------\r\n");

	printf("%s\r\n", data);

	printf("---------------- xml data end ------------------------\r\n");

	acl_xml3_update(xml, buf);

	printf("--------------- walk_xml_node ------------------------\r\n");

	acl_myfree(buf);

	walk_xml_node(xml->root, 0);

	printf("--------------- walk_xml_node end --------------------\r\n");

	printf("Enter any key to continue ...\r\n");
	getchar();

	printf("----------------- lookup meta data -------------------\r\n");
	encoding = acl_xml3_getEncoding(xml);
	type = acl_xml3_getType(xml);
	node = acl_xml3_getElementMeta(xml, "xml-stylesheet");
	if (node)
		href = acl_xml3_getElementAttrVal(node, "href");
	else
		href = NULL;

	printf("xml encoding: %s\r\n", encoding ? encoding : "null");
	printf("xml type: %s\r\n", type ? type : "null");
	printf("xml href: %s\r\n", href ? href : "null");

	printf("----------------- lookup meta data end ---------------\r\n");

	printf("Enter any key to continue ...\r\n");
	getchar();
	test_getElementsByTagName(xml, "root");

	acl_xml3_free(xml);
}

static void usage(const char *procname)
{
	printf("usage: %s -h[help]\r\n"
		" -b benchmark_max\r\n"
		" -P [if parse one xml]\r\n"
		" -f xml_file\r\n"
		" -m[if enable  multiple root xml node, default: no]\r\n"
		" -p[print] data1|data2|data3|data4|data5|data6|data7\r\n"
		" -d[parse] data1|data2|data3|data4|data5|data6|data7\r\n",
		procname);
}

#ifdef WIN32
#define snprintf _snprintf
#endif

int main(int argc, char *argv[])
{
	int   ch, multi_root = 0, benchmark_max = 10000;
	int   parse_one = 0;
	const char *data = __data1;
	const char* root = "root";
	char  filepath[256];

	filepath[0] = 0;

	if (0)
	{
		test1();

		printf("Enter any key to continue ...\r\n");
		getchar();
	}

	while ((ch = getopt(argc, argv, "hp:d:mb:f:P")) > 0) {
		switch (ch) {
		case 'h':
			usage(argv[0]);
			return (0);
		case 'm':
			multi_root = 1;
			break;
		case 'b':
			benchmark_max = atoi(optarg);
			break;
		case 'P':
			parse_one = 1;
			break;
		case 'd':
			if (strcasecmp(optarg, "data2") == 0) {
				data = __data2;
				root = "root";
			} else if (strcasecmp(optarg, "data3") == 0) {
				data = __data3;
				root = "root";
			} else if (strcasecmp(optarg, "data4") == 0) {
				data = __data4;
				root = "request";
			} else if (strcasecmp(optarg, "data5") == 0) {
				data = __data5;
				root = "request";
			} else if (strcasecmp(optarg, "data6") == 0) {
				data = __data6;
				root = "request";
			} else if (strcasecmp(optarg, "data7") == 0) {
				data = __data7;
				root = "request";
			}
			break;
		case 'p':
			if (strcasecmp(optarg, "data1") == 0)
				printf("%s\n", __data1);
			else if (strcasecmp(optarg, "data2") == 0)
				printf("%s\n", __data2);
			else if (strcasecmp(optarg, "data3") == 0)
				printf("%s\n", __data3);
			else if (strcasecmp(optarg, "data4") == 0)
				printf("%s\n", __data4);
			else if (strcasecmp(optarg, "data5") == 0)
				printf("%s\n", __data5);
			else if (strcasecmp(optarg, "data6") == 0)
				printf("%s\n", __data6);
			else if (strcasecmp(optarg, "data7") == 0)
				printf("%s\n", __data7);
			return (0);
		case 'f':
			snprintf(filepath, sizeof(filepath), "%s", optarg);
			break;
		default:
			break;
		}
	}

	if (benchmark_max > 0)
		parse_xml_benchmark(benchmark_max, data);

	if (parse_one)
		parse_xml(data, root, multi_root);

	if (filepath[0] != 0)
		parse_xml_file(filepath);

#ifdef	ACL_MS_WINDOWS
	printf("ok, enter any key to exit ...\n");
	getchar();
#endif
	return 0;
}
