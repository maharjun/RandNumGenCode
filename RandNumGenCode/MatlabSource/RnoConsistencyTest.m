%%
if strcmpi(Build, 'Debug')
	rmpath('../../x64/Release');
	addpath('../../x64/Debug');
elseif strcmpi(Build, 'Release')
	rmpath('../../x64/Debug');
	addpath('../../x64/Release');
end

tic;
try 
	[Set1, Set2] = RandNumGenCode();
catch e
	clear functions;
	throw(e);
end
fprintf('maximum deviation between two sets = %f\n', max(max(abs(Set1-Set2))));
toc;
clear functions;
