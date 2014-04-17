# BTController - Connecting WiiMote to Symbian phone
# Copyright (C) 2010 Juhana Paavola <juhana.paavola@gmail.com>
# 
# This file is part of BTController.
# 
# BTController is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# BTController is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#  You should have received a copy of the GNU Lesser General Public License
# along with BTController.  If not, see <http://www.gnu.org/licenses/>.
 
my $cf = "copytemp.txt";
# QtSDK 1.0 and older
my $qtsdk = "\\NokiaQtSDK\\Symbian\\SDK\\epoc32";
my $qtsdk3 = 0;
my $qtsdkinc3;
my $qtsdkrel3;
my $qtsdkgcceudeb3;
my $qtsdkgcceurel3;	
# QtSDK 1.1
unless(-d $qtsdk){
	$qtsdk = "\\QtSDK\\Symbian\\SDKs\\Symbian1Qt473\\epoc32";
	$qtsdk3 = "\\QtSDK\\Symbian\\SDKs\\Symbian3Qt473\\epoc32";
	$qtsdkinc3 = $qtsdk3."\\include";
	$qtsdkrel3 = $qtsdk3."\\release";
	$qtsdkgcceudeb3 = $qtsdkrel3."\\armv5\\udeb";
	$qtsdkgcceurel3 = $qtsdkrel3."\\armv5\\urel";	
}
my $qtsdkinc = $qtsdk."\\include";
my $qtsdkrel = $qtsdk."\\release";
my $qtsdkgcceudeb = $qtsdkrel."\\gcce\\udeb";
my $qtsdkgcceurel = $qtsdkrel."\\gcce\\urel";
my @inside;

system("call abld build gcce --what > $cf");
open(FILE,$cf) or die "Cannot open $cf:!";
@inside = <FILE>;
close(FILE);

unless(-d $qtsdkgcceudeb){
	system("mkdir $qtsdkgcceudeb");
	if($qtsdk3){
		system("mkdir $qdsdkgccudeb3");
	}
}

unless(-d $qtsdkgcceurel){
	system("mkdir $qtsdkgcceurel");
	if($qtsdk3){
		system("mkdir $qdsdkgccurel3");
	}	
}

# parse abld output and copy files to Qt SDK	
foreach my $line (@inside){	
	chomp($line);
	my @path;
	my $target;
	my $target3 = 0;
	
	if($line =~ /include/i){
		@path = split(/include/,$line);
		$target = $qtsdkinc.$path[1];
		if($qtsdk3){
			$target3 = $qtsdkinc3.$path[1];
		}		
	}
	if($line =~ /gcce/gi){
		@path = split(/release/,$line);
		$target = $qtsdkrel.$path[1];
		if($qtsdk3){
			$target3 = $qtsdkrel3.$path[1];
		}		
	}
	
	if($line =~ /armv/gi){
		@path = split(/release/,$line);
		$target = $qtsdkrel.$path[1];
		if($qtsdk3){
			$target3 = $qtsdkrel3.$path[1];
		}		
	}
	
	if(-e $target){
			unlink($target);
			print("Deleted file: $target\n");			
			if(-e $target3){
				unlink($target3);
				print("Deleted file: $target3\n");			
			}			
	}
	
	print("copy $line -> $target\n");
	system("copy $line $target");			
	if($qtsdk3){
		$target3 =~ s/gcce/armv5/gi;
		print("copy $line -> $target3\n");
		system("copy $line $target3");				
	}
}

system("del $cf");