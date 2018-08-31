CREATE VIEW `truck_dataset_loads_bol` AS
    SELECT
        `l`.`load_nid` AS `load_nid`,
        `l`.`truck_nid` AS `truck_nid`,
        `c`.`field_billoflading_fid` AS `fid`,
        `c`.`field_billoflading_description` AS `description`
    FROM
        (`truck_dataset_loads` `l`
        JOIN `field_data_field_billoflading` `c` ON ((`c`.`entity_id` = `l`.`load_nid`)))
    WHERE
        (`c`.`bundle` = 'dispatch');