%%
if strcmpi(Build, 'Debug')
	rmpath('../../x64/Release');
	addpath('../../x64/Debug');
elseif strcmpi(Build, 'Release')
	rmpath('../../x64/Debug');
	addpath('../../x64/Release');
end

try 
	tic;
	RandNumGenCode();
	toc;
catch e
	clear functions;
	throw(e);
end

clear functions;
