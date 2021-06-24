<?xml version="1.0" encoding="utf-8"?>
<!-- hand-coded with <3 by the 3D Geoinformation group at the TU Delft -->
<!-- https://3d.bk.tudelft.nl -->
<CityModel xmlns:smil20="http://www.w3.org/2001/SMIL20/" xmlns:grp="http://www.opengis.net/citygml/cityobjectgroup/2.0" xmlns:pfx0="http://www.citygml.org/citygml/profiles/base/2.0" xmlns:luse="http://www.opengis.net/citygml/landuse/2.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:frn="http://www.opengis.net/citygml/cityfurniture/2.0" xmlns:smil20lang="http://www.w3.org/2001/SMIL20/Language" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:tex="http://www.opengis.net/citygml/texturedsurface/2.0" xmlns:dem="http://www.opengis.net/citygml/relief/2.0" xmlns:tran="http://www.opengis.net/citygml/transportation/2.0" xmlns:wtr="http://www.opengis.net/citygml/waterbody/2.0" xmlns:xAL="urn:oasis:names:tc:ciq:xsdschema:xAL:2.0" xmlns:bldg="http://www.opengis.net/citygml/building/2.0" xmlns:sch="http://www.ascc.net/xml/schematron" xmlns:app="http://www.opengis.net/citygml/appearance/2.0" xmlns:veg="http://www.opengis.net/citygml/vegetation/2.0" xmlns:gml="http://www.opengis.net/gml" xmlns:gen="http://www.opengis.net/citygml/generics/2.0" xmlns="http://www.opengis.net/citygml/2.0" xsi:schemaLocation="http://www.opengis.net/citygml/2.0 http://schemas.opengis.net/citygml/2.0/cityGMLBase.xsd  http://www.opengis.net/citygml/appearance/2.0 http://schemas.opengis.net/citygml/appearance/2.0/appearance.xsd http://www.opengis.net/citygml/building/2.0 http://schemas.opengis.net/citygml/building/2.0/building.xsd http://www.opengis.net/citygml/cityfurniture/2.0 http://schemas.opengis.net/citygml/cityfurniture/2.0/cityFurniture.xsd http://www.opengis.net/citygml/vegetation/2.0 http://schemas.opengis.net/citygml/vegetation/2.0/vegetation.xsd http://www.opengis.net/citygml/generics/2.0 http://schemas.opengis.net/citygml/generics/2.0/generics.xsd http://www.opengis.net/citygml/transportation/2.0 http://schemas.opengis.net/citygml/transportation/2.0/transportation.xsd http://www.opengis.net/citygml/waterbody/2.0 http://schemas.opengis.net/citygml/waterbody/2.0/waterBody.xsd http://www.opengis.net/citygml/landuse/2.0 http://schemas.opengis.net/citygml/landuse/2.0/landUse.xsd http://www.opengis.net/citygml/relief/2.0 http://schemas.opengis.net/citygml/relief/2.0/relief.xsd">
	
	<gml:boundedBy>
		<gml:Envelope srsDimension="3">
			<gml:lowerCorner>100.00 100.00 0.00 </gml:lowerCorner>
			<gml:upperCorner>400.00 200.00 150.00 </gml:upperCorner>
		</gml:Envelope>
  	</gml:boundedBy>

	<!-- 1st House -->

	<cityObjectMember>
		<bldg:Building>
			<bldg:boundedBy>
				<bldg:GroundSurface>
					<gml:name>Ground</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											100.00 100.00 0.00
											100.00 200.00 0.00
											200.00 200.00 0.00
											200.00 100.00 0.00
											100.00 100.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:GroundSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 1</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											100.00 100.00 0.00
											200.00 100.00 0.00
											200.00 100.00 100.00
											100.00 100.00 100.00
											100.00 100.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 2</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											200.00 100.00 0.00
											200.00 200.00 0.00
											200.00 200.00 100.00
											200.00 100.00 100.00
											200.00 100.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 3</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											200.00 200.00 0.00
											100.00 200.00 0.00
											100.00 200.00 100.00
											200.00 200.00 100.00
											200.00 200.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 4</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											100.00 200.00 0.00
											100.00 100.00 0.00
											100.00 100.00 100.00
											100.00 200.00 100.00
											100.00 200.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 1</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											100.00 100.00 100.00
											200.00 100.00 100.00
											150.00 100.00 150.00
											100.00 100.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 2</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											200.00 200.00 100.00
											100.00 200.00 100.00
											150.00 200.00 150.00
											200.00 200.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 3</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											200.00 100.00 100.00
											200.00 200.00 100.00
											150.00 200.00 150.00
											150.00 100.00 150.00
											200.00 100.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 4</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											100.00 100.00 100.00
											150.00 100.00 150.00
											150.00 200.00 150.00
											100.00 200.00 100.00
											100.00 100.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>

		</bldg:Building>
	</cityObjectMember>

	<!-- 2nd House -->

	<cityObjectMember>
		<bldg:Building>
			<bldg:boundedBy>
				<bldg:GroundSurface>
					<gml:name>Ground</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											200.00 100.00 0.00
											200.00 200.00 0.00
											300.00 200.00 0.00
											300.00 100.00 0.00
											200.00 100.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:GroundSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 1</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											200.00 100.00 0.00
											300.00 100.00 0.00
											300.00 100.00 100.00
											200.00 100.00 100.00
											200.00 100.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 2</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											300.00 100.00 0.00
											300.00 200.00 0.00
											300.00 200.00 100.00
											300.00 100.00 100.00
											300.00 100.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 3</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											300.00 200.00 0.00
											200.00 200.00 0.00
											200.00 200.00 100.00
											300.00 200.00 100.00
											300.00 200.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 4</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											200.00 200.00 0.00
											200.00 100.00 0.00
											200.00 100.00 100.00
											200.00 200.00 100.00
											200.00 200.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 1</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											200.00 100.00 100.00
											300.00 100.00 100.00
											250.00 100.00 150.00
											200.00 100.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 2</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											300.00 200.00 100.00
											200.00 200.00 100.00
											250.00 200.00 150.00
											300.00 200.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 3</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											300.00 100.00 100.00
											300.00 200.00 100.00
											250.00 200.00 150.00
											250.00 100.00 150.00
											300.00 100.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 4</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											200.00 100.00 100.00
											250.00 100.00 150.00
											250.00 200.00 150.00
											200.00 200.00 100.00
											200.00 100.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>
			
		</bldg:Building>
	</cityObjectMember>

	<!-- 3rd House -->

	<cityObjectMember>
		<bldg:Building>
			<bldg:boundedBy>
				<bldg:GroundSurface>
					<gml:name>Ground</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											300.00 100.00 0.00
											300.00 200.00 0.00
											400.00 200.00 0.00
											400.00 100.00 0.00
											300.00 100.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:GroundSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 1</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											300.00 100.00 0.00
											400.00 100.00 0.00
											400.00 100.00 100.00
											300.00 100.00 100.00
											300.00 100.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 2</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											400.00 100.00 0.00
											400.00 200.00 0.00
											400.00 200.00 100.00
											400.00 100.00 100.00
											400.00 100.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 3</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											400.00 200.00 0.00
											300.00 200.00 0.00
											300.00 200.00 100.00
											400.00 200.00 100.00
											400.00 200.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:WallSurface>
					<gml:name>Wall 4</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											300.00 200.00 0.00
											300.00 100.00 0.00
											300.00 100.00 100.00
											300.00 200.00 100.00
											300.00 200.00 0.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 1</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											300.00 100.00 100.00
											400.00 100.00 100.00
											350.00 100.00 150.00
											300.00 100.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 2</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											400.00 200.00 100.00
											300.00 200.00 100.00
											350.00 200.00 150.00
											400.00 200.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 3</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											400.00 100.00 100.00
											400.00 200.00 100.00
											350.00 200.00 150.00
											350.00 100.00 150.00
											400.00 100.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>

			<bldg:boundedBy>
				<bldg:RoofSurface>
					<gml:name>Roof 4</gml:name>
					<bldg:MultiSurface>
						<gml:surfaceMember>
							<gml:Polygon>
								<gml:exterior>
									<gml:LinearRing>
										<gml:posList>
											300.00 100.00 100.00
											350.00 100.00 150.00
											350.00 200.00 150.00
											300.00 200.00 100.00
											300.00 100.00 100.00
										</gml:posList>
									</gml:LinearRing>
								</gml:exterior>
							</gml:Polygon>
						</gml:surfaceMember>
					</bldg:MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>
			
		</bldg:Building>
	</cityObjectMember>

</CityModel>
