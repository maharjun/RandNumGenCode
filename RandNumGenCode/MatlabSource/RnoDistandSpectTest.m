%%
if strcmpi(Build, 'Debug')
	rmpath('../../x64/Release');
	addpath('../../x64/Debug');
elseif strcmpi(Build, 'Release')
	rmpath('../../x64/Debug');
	addpath('../../x64/Release');
end
zeromeankurt = @(x) mean(x.^4)/(mean(x.^2))^2 - 3;
tic;
try 
	[Set1] = RandNumGenCode();
catch e
	clear functions;
	throw(e);
end

fprintf('mean = %f\n', mean(reshape(Set1(:, 80:80:end), [], 1)));
fprintf('kurtosis = %f\n', zeromeankurt(reshape(Set1(:, 50:50:end), [], 1)));
x = reshape(Set1(:,51:end)', [], 1);
X = reshape(Set1(:,50:end-1)', [], 1);
a = (X'*X)\X'*x;
clear X x;
fprintf('estimated a = %f\n', a);
toc;
clear functions;
