import zipfile;
import copy;
from xml.dom.minidom import parse, parseString;


class Spreadsheet:
	def __init__(self, filename=None):
		# Load the spreadsheet and parse the filename if given
		if (filename != None):
			self.filename= filename;
			self.parse();


	def parse(self):
		"""Parse the ODS and load the results into this spreadsheet object."""

		z = zipfile.ZipFile(self.filename, 'r');
		c = z.read('content.xml');

		dom = parseString(c);

		self.parse_dom(dom);

		dom.unlink();

		
	def parse_dom(self, dom):
		"""DOM is read. Now load data from DOM."""

		# Setup empty key sets
		self.sheets = {};

		sheets = dom.getElementsByTagName("table:table");
		for i in sheets:
			self.parse_sheet(i);

	def parse_sheet(self, table):
		"""Hand a sheet - read it."""
		
		# First find the name of the sheet
		name = table.getAttribute('table:name');

		# Now load the table
		#columns = table.getElementsByTagName("table:table-column");
		#ncolsstr = columns[0].getAttribute("table:number-columns-repeated");
		#ncols = 1;
		#if ncolsstr != "":
		#	ncols = int(str(ncolsstr));
		
		sheet = [];
		#for i in range(ncols):	# Make enough empty columns
		#	sheet.append([]);
		
		# Now for each row in the sheet
		rows = table.getElementsByTagName("table:table-row");
		for r in rows:
			cells = r.getElementsByTagName("table:table-cell");
			numnew = 0;
			for c in cells:
				n = 1;
				if c.hasAttribute("table:number-columns-repeated"):
					n = int(c.getAttribute("table:number-columns-repeated"));
				numnew += n;

			# Check we have enough columns for this row
			numnew = numnew - len(sheet);
			if numnew > 0:
				emptycol = [];
				if(len(sheet)):	# We have already started another column so need to make this column up to length
					for i in sheet[0]:
						emptycol.append('');
				for i in range(numnew):
					sheet.append(copy.copy(emptycol));

			inspos = 0;
			for i in range(len(cells)):
				c = cells[i];
				# Check what type of cell this is
				val = "";
				if c.hasAttribute("office:value"):
					val = c.getAttribute("office:value");
					if c.getAttribute("office:value-type") == "float":
						val = float(val);
				else:
					# Try reading any text it might contain
					t = c.getElementsByTagName("text:p");
					if(len(t)):
						cn = t[0].childNodes;
						if(len(cn)):
							val = cn[0].data;
			
				# There may be a repeated value
				ntimes = 1;
				if c.hasAttribute("table:number-columns-repeated"):
					ntimes = int(c.getAttribute("table:number-columns-repeated"));
				for l in range(ntimes):
					sheet[inspos].append(val);
					inspos+=1;
				

		self.sheets[name] = sheet;